#include "dongle.h"


Dongle::Dongle()
{
    currentl2w = NULL;
    state = STATE_DISCONNECTED;
    handle = NULL;
    activity = DO_NORMAL;
    target_device_state         = 0b00000000;
    current_device_state = 0b00000000;
    target_group_state         = 0b00000000;
    current_group_state = 0b00000000;


    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);
    bEnableVersion2 = sett.value("MAIN/EnableVersion2", 0).toBool();

    for(int i = 0; i < KEYNUM; i++) key_transfer_state[i] = false;
}

void Dongle::spin()
{
    qDebug("Spin dongle");
    if(state == STATE_DISCONNECTED){
        connect();
    } else {

        if(currentl2w != NULL){
            if(state == STATE_ON || state == STATE_OFF) {
                switch (activity) {
                case DO_WRITEALLTODONGLE:
                    sendCMD_DELETE_ALL();
                    #ifdef WIN32
                        Sleep(150);
                    #else
                        usleep(150*1000);
                    #endif
                    for(int i = 0; i< 48; i++){
                        if(currentl2w->getCurrentSettings()->condition[i]->FSet){
                            sendKeyToDongle(i);
                            #ifdef WIN32
                                Sleep(50);
                            #else
                                usleep(50*1000);
                            #endif
                        }
                    }
                    break;
                case DO_WRITEKEYTODONGLE:
                    for(int i = 0; i< 48; i++){
                        if(!key_transfer_state[i]){

                            sendKeyToDongle(i);
                            #ifdef WIN32
                                Sleep(50);
                            #else
                                usleep(50*1000);
                            #endif
                        }
                    }
                    break;
                case DO_SETDEVICESTATE:
                    sendCMD_SET_STATE();
                    break;
                default:
                    break;
                }
                activity = DO_NORMAL;

                recieve_status() ;
                if(bEnableVersion2) {
                    sendCMD_SET_SKILL_STATE();
                } else {
                    transmitHPCPMP();
                    sendCMD_SET_TARGET_STATE();
                }
            }
        }

    }
}

void Dongle::process()
{
    qDebug("Start dongle checker");

    while(1){
        QTime updateTime;
        updateTime.start();
        int delta;

        try {
            spin();
        } catch(...) {
            qDebug("Spin dongle failed");
            state = STATE_DISCONNECTED;
        }

        emit showStatus(current_device_state, updateTime.elapsed());
        delta = 20 - updateTime.elapsed();
        delta = (delta > 0)?delta:0;
        #ifdef WIN32
            Sleep(delta);
        #else
            usleep(delta*1000);
        #endif

    }
    emit finished();
}

void Dongle::setActiveL2W(L2Window* l2w)
{
    qDebug("Dongle::setActiveL2W");
    currentl2w = l2w;
}


void Dongle::recieve_status() {
    qDebug("Dongle::recieve_status");
    unsigned char buf[9];
    memset(buf,0,sizeof(buf));

    buf[0] = 0;
    int res;
    try {
        res =  hid_get_input_report(handle, buf, sizeof(buf));
    } catch(...) {
        res = -1;
        state = STATE_DISCONNECTED;
    }

    if (res == -1)
    {
        qDebug("Ошибка при  приёме данных");
        disconnect();
    } else {
        current_device_state = buf[1] & DEVICE_MASK;
        if((current_device_state & (1 << DEVICE_STATUS)) == 0) {
            state = STATE_OFF;
        } else {
            state = STATE_ON;
        }
        current_group_state = buf[2] & DEVICE_MASK;

    }
}


int Dongle::connect()
{
    qDebug("Dongle::connect()");

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    //handle = hid_open(0x03eb, 0x204d, L"12345");
    try {
        handle = hid_open(0x03eb, 0x204d, NULL);
    } catch(...) {
        qDebug("unable to open device exeption\n");
        state = STATE_DISCONNECTED;
    }
    if (!handle) {
        qDebug("unable to open device\n");
        state = STATE_DISCONNECTED;
        return state;
    }
    // Set the hid_read() function to be non-blocking.

    try {
        hid_set_nonblocking(handle, 1);
    } catch(...) {
       qDebug("unable to set hid_set_nonblocking\n");;
       state = STATE_DISCONNECTED;
    }
    state = STATE_OFF;
    activity = DO_WRITEALLTODONGLE;
    return state;
}

int Dongle::disconnect()
{
    qDebug("Dongle::disconnect()");

    try {
        hid_close(handle);
    } catch(...) {
        ;;
        qDebug("hid_close failed\n");
        state = STATE_DISCONNECTED;

    }

    /* Free static HIDAPI objects. */

    try {
        hid_exit();
    } catch(...) {
        ;;
        qDebug("hid_exit failed\n");
        state = STATE_DISCONNECTED;

    }

    state = STATE_DISCONNECTED;
    return STATE_DISCONNECTED;
}



int Dongle::send_command(int device, int command, unsigned char* cmd_arg)
{
    qDebug("Dongle::send_command(int device, int command, unsigned char* cmd_arg)command: %d", command);
    int res;
    unsigned char buf[HID_REPORT_SIZE];

    if(state != STATE_DISCONNECTED && state != STATE_TRANSMIT_CONF){
        // Set up the command buffer.
        memset(buf,0,sizeof(buf));
        buf[0] = 0x00;
        buf[1] = device;  //INTERFACE_ID_Keyboard
        buf[2] = command;// CMD_SET_HPCPMP
        buf[3] = cmd_arg[0];
        buf[4] = cmd_arg[1];
        buf[5] = cmd_arg[2];
        buf[6] = cmd_arg[3];
        buf[7] = cmd_arg[4];
        buf[8] = cmd_arg[5];

        int current_state = state;
        state = STATE_TRANSMIT_CONF;

        try {
            res = hid_write(handle, buf, 9);
        } catch(...) {
            res = -1;
        }
        if (res < 0) {
            qDebug("Unable to write()\n");
            return disconnect();
        }
        state = current_state;
    }
    return(state);
}



int Dongle::set_key_report(unsigned char key_code, bool Ctrl, bool Shift)
{
    qDebug("int Dongle::send_key(QString Key, bool Ctrl, bool Shift)command");

    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
     // Modifiers:
    if(Ctrl) hpbuf[5]  |= HID_KEYBOARD_MODIFIER_LEFTCTRL;
    if(Shift) hpbuf[5]  |= HID_KEYBOARD_MODIFIER_LEFTSHIFT;
    send_command(SERVICE_CONFIG, key_code, hpbuf);
    return 1;
}

int Dongle::send_key(unsigned char key_code, bool Ctrl, bool Shift)
{
    qDebug("int Dongle::send_key(QString Key, bool Ctrl, bool Shift)command");
    set_key_report(key_code, Ctrl, Shift);
    #ifdef WIN32
        Sleep(60);
    #else
        usleep(60*1000);
    #endif
    set_key_report(0x00, false, false);

    return(state);
}




void Dongle::transmitHPCPMP(){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    qDebug("Dongle::transmitHPCPMP");
    memset(hpbuf,0,sizeof(hpbuf));
    qDebug("idHP: %d, idMP: %d, idMobHP: %d, idMobMP: %d",
           getXP(idHP), getXP(idMP), getXP(idMobHP), getXP(idMobMP));
    hpbuf[0] = (((getXP(idMobMP)<XP_ERR) && (getXP(idMobMP) > 0)) ||(getXP(idMobHP)>100) )? 0: getXP(idMobHP);
    hpbuf[1] = getXP(idHP);
    hpbuf[2] = getXP(idCP);
    hpbuf[3] = getXP(idMP);
    hpbuf[4] = getXP(idVP);
    hpbuf[5] = getXP(idMobMP);

    qDebug("idHP: %d, idMP: %d, idMobHP: %d", hpbuf[1], hpbuf[3], hpbuf[0]);

    send_command(SERVICE_CONFIG, CMD_SET_HPCPMP, hpbuf);

}

void Dongle::sendCMD_SET_STATE() {
    qDebug("Dongle::sendCMD_SET_STATE");
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    hpbuf[0] = target_group_state;
    send_command(SERVICE_DEVICE, target_device_state, hpbuf);
}


void Dongle::sendCMD_SET_TARGET_STATE(){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    qDebug("Dongle::sendCMD_SET_TARGET_STATE");
    //memset(hpbuf,0,sizeof(hpbuf));
    hpbuf[0] = getTargetType();
    hpbuf[1] = getStarState();
    hpbuf[2] = 0;
    hpbuf[3] = 0;
    hpbuf[4] = 0;
    hpbuf[5] = 0;
    for(int i = 0; i < (HID_REPORT_SIZE-5)*8; i++){  //32 keys
        if(currentl2w->getConditionState(i)){
            if(currentl2w->isSkillRdy(i)) {
                int buf_byte = i >> 3;
                int buf_bit = i - (buf_byte << 3);
                unsigned char t = hpbuf[2+buf_byte] | ((unsigned char)(1 << buf_bit));
                hpbuf[2+buf_byte] =   t;
                qDebug("Skill  %d = %d * 8 + %d = %d", i, buf_byte, buf_bit, hpbuf[2+buf_byte]);
            }
        }
    }
    qDebug("TargetType: %d, StarState: %d Skill State %d %d %d %d", hpbuf[0], hpbuf[1], hpbuf[2], hpbuf[3], hpbuf[4], hpbuf[5]);
    send_command(SERVICE_CONFIG, CMD_SET_TARGET_STATE, hpbuf);
}


void Dongle::sendCMD_SET_SKILL_STATE(){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    qDebug("Dongle::sendCMD_SET_SKILL_STATE");
    //memset(hpbuf,0,sizeof(hpbuf));
    hpbuf[0] = 0;
    hpbuf[1] = 0;
    hpbuf[2] = 0;
    hpbuf[3] = 0;
    hpbuf[4] = 0;
    hpbuf[5] = 0;
    for(int i = 0; i < (HID_REPORT_SIZE-3)*8; i++){  //48 keys
        if(currentl2w->getConditionState(i)){
            if(currentl2w->isSkillRdy(i)) {
                int buf_byte = i >> 3;
                int buf_bit = i - (buf_byte << 3);
                unsigned char t = hpbuf[buf_byte] | ((unsigned char)(1 << buf_bit));
                hpbuf[buf_byte] =   t;
                qDebug("Skill  %d = %d * 8 + %d = %d", i, buf_byte, buf_bit, hpbuf[buf_byte]);
            }
        }
    }
    qDebug("Skill State %d %d %d %d %d %d", hpbuf[0], hpbuf[1], hpbuf[2], hpbuf[3], hpbuf[4], hpbuf[5]);
    send_command(SERVICE_CONFIG, CMD_SET_SKILL_STATE, hpbuf);
}



void Dongle::sendCMD_ADD_NODE(unsigned char key_code, float PauseTime, float ReleaseTime, float ConditionTime, unsigned int groups, bool Ctrl, bool Shift){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));

   int p = (int)(PauseTime*2);  //; 0.5s grade
   int r = (int)(ReleaseTime*10);  //; 0.1s grade
   int c = (int)(ConditionTime*2); //; 0.5s grade

   p = (p>0xFF)? 0xFF: p;
   r = (r>0xFFFF)? 0xFFFF: r;
   c = (c>0xFF)? 0xFF: c;

    hpbuf[0] = key_code;                                     //2
    hpbuf[1] = p; // $xPauseTime                                        //3
    hpbuf[2] = r & 0xFF; // BitAND(0xFF, $xReleaseTime)                 //4
    hpbuf[3] = (r >> 8) & 0xFF; // BitAND(0xFF, $xReleaseTime/256)      //5
    hpbuf[4] = c; // $xConditionTime                                    //6
    hpbuf[5] = groups;                                                  //7
    if(Ctrl)      hpbuf[5] |= 0b00000010; // Ctrl
    if(Shift)     hpbuf[5] |= 0b00000001; // Shift

    //qDebug("CMD_ADD_NODE %s, Cooldown (0.1s) %d, Abs (0.5s) %d, Cond (0.5s) %d", Key.toStdString().c_str(), hpbuf[2]+hpbuf[3]*256, hpbuf[1], hpbuf[4]);
    send_command(SERVICE_CONFIG, CMD_ADD_NODE, hpbuf);
}

void Dongle::sendCMD_DELETE_NODE(unsigned char key_code){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));

    hpbuf[0] = key_code;
    send_command(SERVICE_CONFIG, CMD_DELETE_NODE, hpbuf);
}


void Dongle::sendCMD_DELETE_ALL(){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    send_command(SERVICE_CONFIG, CMD_DELETE_ALL, hpbuf);
}


void Dongle::sendCMD_ADD_NODE_CONDITION(unsigned char key_code, unsigned char Type, unsigned char Min, unsigned char Max){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));

    hpbuf[0] = key_code;

    switch(Type){
    case idCP:
        hpbuf[1] = 2; //    Global Const $t_playerCP = 2 ;/** Player CP */
        break;
    case idHP:
        hpbuf[1] = 1; //    Global Const $t_playerHP = 1 ;/** Player HP */
        break;
    case idMP:
        hpbuf[1] = 3; //    Global Const $t_playerMP = 3 ;/** Player MP */
        break;
    case idMobHP:
        hpbuf[1] = 0; //    Global Const $t_mobHP = 0 ;/** Mob HP */
        break;
    case idMobMP:
        hpbuf[1] = 5; //
        break;
    case idVP:
        hpbuf[1] = 4; //
        break;
    case idTargetType:
        hpbuf[1] = 6; //
        break;
    case idCheckSkillType:
        hpbuf[1] = 8; //
        break;
    default:
        hpbuf[1] = 7; //
        break;
    }

    hpbuf[2] = (Min <=100)? Min: 0xFF; //$Min
    hpbuf[3] = (Max <=100)? Max: 0xFF;  // $Max

    send_command(SERVICE_CONFIG, CMD_ADD_NODE_CONDITION, hpbuf);
}

unsigned char Dongle::getXP(int idXP){
    if(currentl2w == NULL){
        return XP_ERR;
    } else {
        return currentl2w->getXP(idXP);
    }
}

unsigned char Dongle::getTargetType(){
    if(currentl2w == NULL){
        return NOTARGET;
    } else {
        return currentl2w->getTargetType();
    }
}

unsigned char Dongle::getStarState(){
    if(currentl2w == NULL){
        return 0;
    } else {
        if(currentl2w->getStarState()){
            return 1;
        } else {
            return 0;
        }
    }
}

void Dongle::sendKeyToDongle(int condition_index){
    qDebug("Dongle::sendKeyToDongle(int condition_index, bool state)");

    if(currentl2w == NULL)return;
    KeyCondition * cond = currentl2w->getCurrentSettings()->condition[condition_index];

     if((currentl2w->getConditionState(condition_index))){
        sendCMD_ADD_NODE(
                     cond->KeyCode,
                     cond->conditionf[idPause],
                     cond->conditionf[idCoolDown],
                     cond->conditionf[idCondition],
                     cond->getGroupsBinaryCondition() << 2,
                    cond->ctrl,
                    cond->shift
                    );

        sendCMD_ADD_NODE_CONDITION(
                        cond->KeyCode,
                        idCheckSkillType,
                        condition_index, // bit```
                        0  // unused
                  );
        qDebug("sendCMD_ADD_NODE_CONDITION Skill (%d): %d", idCheckSkillType, condition_index);

        if(bEnableVersion2) return;
        for(int j = idCP; j < BARNUM; j++ )
        {
            if(cond->conditioni[j] < 100 ||
               cond->conditioni[BARNUM+j] < 100 )
            {
                    sendCMD_ADD_NODE_CONDITION(
                                cond->KeyCode,
                                j,
                                cond->conditioni[j],
                                cond->conditioni[BARNUM+j]
                        );
            }
        }
        if(
            (((cond->getTargetTypeBinaryCondition() & 0b00001111 ) != 0b00001111) || // Target Condition
            ((cond->getStarStatusBinaryCondition() & 0b00000010) > 0))// Star Condition
        ){
            sendCMD_ADD_NODE_CONDITION(
                            cond->KeyCode,
                            idTargetType,
                            cond->getTargetTypeBinaryCondition(), // Target Condition
                            cond->getStarStatusBinaryCondition()  // Star Condition
                      );
        }
    } else {
        sendCMD_DELETE_NODE(cond->KeyCode);
    }
    key_transfer_state[condition_index] = true;
}





void Dongle::doSaveAllToDongle(){
    activity = DO_WRITEALLTODONGLE;
}

void Dongle::doSendKeyToDongle(int condition_index){
    qDebug("Dongle::doSendKeyToDongle(int condition_index)");
    key_transfer_state[condition_index] = false;
    activity = DO_WRITEKEYTODONGLE;
}
