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



    for(int i = 0; i < KEYNUM; i++) key_transfer_state[i] = false;
}

void Dongle::spin()
{
    qDebug("Spin dongle");
    if(state == STATE_DISCONNECTED){
        connect();
    } else {
        if(state == STATE_ON || state == STATE_OFF) {
            if(currentl2w != NULL){
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
                default:
                    break;
                }
                sendCMD_SET_SKILL_STATE();
            }
            if(activity == DO_JUMPTOBOOTLOADER) {
                sendCMD_JUMP_TO_BOOTLOADER();
                disconnect();
            } else if(activity == DO_SETDEVICESTATE) {
                sendCMD_SET_STATE();
            }
            recieve_status() ;
            activity = DO_NORMAL;
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

        emit showStatus(current_device_state, current_group_state, updateTime.elapsed());
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
    unsigned char buf[IN_REPORT_SIZE];

    if(state == STATE_DISCONNECTED) return;
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
        current_group_state = buf[2];

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
    unsigned char buf[OUT_REPORT_SIZE];

    if(state != STATE_DISCONNECTED && state != STATE_TRANSMIT_CONF){
        // Set up the command buffer.
        memset(buf,0,sizeof(buf));
        buf[0] = 0x00;
        buf[1] = device;  //INTERFACE_ID_Keyboard
        buf[2] = command;// CMD_SET_HPCPMP
        for(int i = 0; i< OUT_REPORT_SIZE-3; i++) buf[i+3] = cmd_arg[i];
        int current_state = state;
        state = STATE_TRANSMIT_CONF;

        try {
            res = hid_write(handle, buf, sizeof(buf));
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

    unsigned char hpbuf[OUT_REPORT_SIZE-3];
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


void Dongle::sendCMD_SET_STATE() {
    qDebug("Dongle::sendCMD_SET_STATE");
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    hpbuf[0] = target_group_state;
    send_command(SERVICE_DEVICE, target_device_state, hpbuf);
}


void Dongle::sendCMD_SET_SKILL_STATE(){
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    qDebug("Dongle::sendCMD_SET_SKILL_STATE");
    //memset(hpbuf,0,sizeof(hpbuf));
    hpbuf[0] = 0;
    hpbuf[1] = 0;
    hpbuf[2] = 0;
    hpbuf[3] = 0;
    hpbuf[4] = 0;
    hpbuf[5] = 0;
    hpbuf[6] = 0;

    for(int i = 0; i < 48; i++){  //48 keys
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



void Dongle::sendCMD_ADD_NODE(int index, unsigned char key_code, float PauseTime, float ReleaseTime, float ConditionTime, unsigned int groups, bool Ctrl, bool Shift){
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));

    hpbuf[0] = index;                                                           //0
    hpbuf[1] = key_code;                                                        //1
    *((uint32_t*)(hpbuf+2)) = (uint32_t)(PauseTime*1000);  //; 1 ms grade           //2, 3, 4, 5
    *((uint32_t*)(hpbuf+6)) = (uint32_t)(ReleaseTime*1000);  //; 1 ms grade         //6, 7, 8, 9
    *((uint32_t*)(hpbuf+10)) = (uint32_t)(ConditionTime*1000);  //; 1 ms grade      //10, 11, 12, 13
    hpbuf[14] = groups;                                                         //14
    hpbuf[15] = 0;                                                              //15

    if(Ctrl)      hpbuf[15] |= 0b00001000; // Ctrl
    if(Shift)     hpbuf[15] |= 0b00000100; // Shift


    //qDebug("CMD_ADD_NODE %s, Cooldown (0.1s) %d, Abs (0.5s) %d, Cond (0.5s) %d", Key.toStdString().c_str(), hpbuf[2]+hpbuf[3]*256, hpbuf[1], hpbuf[4]);
    send_command(SERVICE_CONFIG, CMD_ADD_NODE, hpbuf);
}

void Dongle::sendCMD_DELETE_NODE(int index){
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    hpbuf[0] = index & 0xFF;
    send_command(SERVICE_CONFIG, CMD_DELETE_NODE, hpbuf);
}


void Dongle::sendCMD_DELETE_ALL(){
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    send_command(SERVICE_CONFIG, CMD_DELETE_ALL, hpbuf);
}

void Dongle::sendCMD_JUMP_TO_BOOTLOADER(){
    unsigned char hpbuf[OUT_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    send_command(SERVICE_CONFIG, CMD_JUMP_TO_BOOTLOADER, hpbuf);
}


void Dongle::sendKeyToDongle(int condition_index){
    qDebug("Dongle::sendKeyToDongle(int condition_index, bool state)");

    if(currentl2w == NULL)return;
    KeyCondition * cond = currentl2w->getCurrentSettings()->condition[condition_index];

     if((currentl2w->getConditionState(condition_index))){
        sendCMD_ADD_NODE(
                    condition_index,
                    cond->KeyCode,
                     cond->conditionf[idPause],
                     cond->conditionf[idCoolDown],
                     cond->conditionf[idCondition],
                     cond->getGroupsBinaryCondition(),
                    cond->ctrl,
                    cond->shift
                    );
    } else {
        sendCMD_DELETE_NODE(condition_index);
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

void Dongle::doJumpToBootloader(){
    activity = DO_JUMPTOBOOTLOADER;
}
