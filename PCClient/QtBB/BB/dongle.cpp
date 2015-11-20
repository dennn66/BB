#include "dongle.h"

char* Dongle::KeyMnemonicDB[219] = {"ERROR_ROLLOVER", 	"POST_FAIL", 	"ERROR_UNDEFINED", 	"A", 	"B", 	"C", 	"D", 	"E", 	"F", 	"G", 	"H", 	"I", 	"J", 	"K", 	"L", 	"M", 	"N", 	"O", 	"P", 	"Q", 	"R", 	"S", 	"T", 	"U", 	"V", 	"W", 	"X", 	"Y", 	"Z", 	"1", 	"2", 	"3", 	"4", 	"5", 	"6", 	"7", 	"8", 	"9", 	"0", 	"ENTER", 	"ESCAPE", 	"BACKSPACE", 	"TAB", 	"SPACE", 	"-", 	"=", 	"[", 	"]", 	"\\", 	"`", 	";", 	"", 	"GRAVE_ACCENT_AND_TILDE", 	",", 	".", 	"/", 	"CAPS_LOCK", 	"F1", 	"F2", 	"F3", 	"F4", 	"F5", 	"F6", 	"F7", 	"F8", 	"F9", 	"F10", 	"F11", 	"F12", 	"PRINT_SCREEN", 	"SCROLL_LOCK", 	"PAUSE", 	"INSERT", 	"HOME", 	"PAGE_UP", 	"DELETE", 	"END", 	"PAGE_DOWN", 	"RIGHT_ARROW", 	"LEFT_ARROW", 	"DOWN_ARROW", 	"UP_ARROW", 	"NUM_LOCK", 	"KEYPAD_SLASH", 	"KEYPAD_ASTERISK", 	"KEYPAD_MINUS", 	"KEYPAD_PLUS", 	"KEYPAD_ENTER", 	"P1", 	"P2", 	"P3", 	"P4", 	"P5", 	"P6", 	"P7", 	"P8", 	"P9", 	"P0", 	"KEYPAD_DOT_AND_DELETE", 	"NON_US_BACKSLASH_AND_PIPE", 	"APPLICATION", 	"POWER", 	"KEYPAD_EQUAL_SIGN", 	"F13", 	"F14", 	"F15", 	"F16", 	"F17", 	"F18", 	"F19", 	"F20", 	"F21", 	"F22", 	"F23", 	"F24", 	"EXECUTE", 	"HELP", 	"MANU", 	"SELECT", 	"STOP", 	"AGAIN", 	"UNDO", 	"CUT", 	"COPY", 	"PASTE", 	"FIND", 	"MUTE", 	"VOLUME_UP", 	"VOLUME_DOWN", 	"LOCKING_CAPS_LOCK", 	"LOCKING_NUM_LOCK", 	"LOCKING_SCROLL_LOCK", 	"KEYPAD_COMMA", 	"KEYPAD_EQUAL_SIGN_AS400", 	"INTERNATIONAL1", 	"INTERNATIONAL2", 	"INTERNATIONAL3", 	"INTERNATIONAL4", 	"INTERNATIONAL5", 	"INTERNATIONAL6", 	"INTERNATIONAL7", 	"INTERNATIONAL8", 	"INTERNATIONAL9", 	"LANG1", 	"LANG2", 	"LANG3", 	"LANG4", 	"LANG5", 	"LANG6", 	"LANG7", 	"LANG8", 	"LANG9", 	"ALTERNATE_ERASE", 	"SISREQ", 	"CANCEL", 	"CLEAR", 	"PRIOR", 	"RETURN", 	"SEPARATOR", 	"OUT", 	"OPER", 	"CLEAR_AND_AGAIN", 	"CRSEL_ANDPROPS", 	"EXSEL", 	"KEYPAD_00", 	"KEYPAD_000", 	"THOUSANDS_SEPARATOR", 	"DECIMAL_SEPARATOR", 	"CURRENCY_UNIT", 	"CURRENCY_SUB_UNIT", 	"KEYPAD_OPENING_PARENTHESIS", 	"KEYPAD_CLOSING_PARENTHESIS", 	"KEYPAD_OPENING_BRACE", 	"KEYPAD_CLOSING_BRACE", 	"KEYPAD_TAB", 	"KEYPAD_BACKSPACE", 	"KEYPAD_A", 	"KEYPAD_B", 	"KEYPAD_C", 	"KEYPAD_D", 	"KEYPAD_E", 	"KEYPAD_F", 	"KEYPAD_XOR", 	"KEYPAD_CARET", 	"KEYPAD_PERCENTAGE", 	"KEYPAD_LESS_THAN_SIGN", 	"KEYPAD_GREATER_THAN_SIGN", 	"KEYPAD_AMP", 	"KEYPAD_AMP_AMP", 	"KEYPAD_PIPE", 	"KEYPAD_PIPE_PIPE", 	"KEYPAD_COLON", 	"KEYPAD_HASHMARK", 	"KEYPAD_SPACE", 	"KEYPAD_AT", 	"KEYPAD_EXCLAMATION_SIGN", 	"KEYPAD_MEMORY_STORE", 	"KEYPAD_MEMORY_RECALL", 	"KEYPAD_MEMORY_CLEAR", 	"KEYPAD_MEMORY_ADD", 	"KEYPAD_MEMORY_SUBTRACT", 	"KEYPAD_MEMORY_MULTIPLY", 	"KEYPAD_MEMORY_DIVIDE", 	"KEYPAD_PLUS_AND_MINUS", 	"KEYPAD_CLEAR", 	"KEYPAD_CLEAR_ENTRY", 	"KEYPAD_BINARY", 	"KEYPAD_OCTAL", 	"KEYPAD_DECIMAL", 	"KEYPAD_HEXADECIMAL", 	"LEFT_CONTROL", 	"LEFT_SHIFT", 	"LEFT_ALT", 	"LEFT_GUI", 	"RIGHT_CONTROL", 	"RIGHT_SHIFT", 	"RIGHT_ALT", 	"RIGHT_GUI", 	"MOUSELEFT"};
unsigned char Dongle::KeyCodesDB[219] = {0x01, 	0x02, 	0x03, 	0x04, 	0x05, 	0x06, 	0x07, 	0x08, 	0x09, 	0x0A, 	0x0B, 	0x0C, 	0x0D, 	0x0E, 	0x0F, 	0x10, 	0x11, 	0x12, 	0x13, 	0x14, 	0x15, 	0x16, 	0x17, 	0x18, 	0x19, 	0x1A, 	0x1B, 	0x1C, 	0x1D, 	0x1E, 	0x1F, 	0x20, 	0x21, 	0x22, 	0x23, 	0x24, 	0x25, 	0x26, 	0x27, 	0x28, 	0x29, 	0x2A, 	0x2B, 	0x2C, 	0x2D, 	0x2E, 	0x2F, 	0x30, 	0x31, 	0x32, 	0x33, 	0x34, 	0x35, 	0x36, 	0x37, 	0x38, 	0x39, 	0x3A, 	0x3B, 	0x3C, 	0x3D, 	0x3E, 	0x3F, 	0x40, 	0x41, 	0x42, 	0x43, 	0x44, 	0x45, 	0x46, 	0x47, 	0x48, 	0x49, 	0x4A, 	0x4B, 	0x4C, 	0x4D, 	0x4E, 	0x4F, 	0x50, 	0x51, 	0x52, 	0x53, 	0x54, 	0x55, 	0x56, 	0x57, 	0x58, 	0x59, 	0x5A, 	0x5B, 	0x5C, 	0x5D, 	0x5E, 	0x5F, 	0x60, 	0x61, 	0x62, 	0x63, 	0x64, 	0x65, 	0x66, 	0x67, 	0x68, 	0x69, 	0x6A, 	0x6B, 	0x6C, 	0x6D, 	0x6E, 	0x6F, 	0x70, 	0x71, 	0x72, 	0x73, 	0x74, 	0x75, 	0x76, 	0x77, 	0x78, 	0x79, 	0x7A, 	0x7B, 	0x7C, 	0x7D, 	0x7E, 	0x7F, 	0x80, 	0x81, 	0x82, 	0x83, 	0x84, 	0x85, 	0x86, 	0x87, 	0x88, 	0x89, 	0x8A, 	0x8B, 	0x8C, 	0x8D, 	0x8E, 	0x8F, 	0x90, 	0x91, 	0x92, 	0x93, 	0x94, 	0x95, 	0x96, 	0x97, 	0x98, 	0x99, 	0x9A, 	0x9B, 	0x9C, 	0x9D, 	0x9E, 	0x9F, 	0xA0, 	0xA1, 	0xA2, 	0xA3, 	0xA4, 	0xB0, 	0xB1, 	0xB2, 	0xB3, 	0xB4, 	0xB5, 	0xB6, 	0xB7, 	0xB8, 	0xB9, 	0xBA, 	0xBB, 	0xBC, 	0xBD, 	0xBE, 	0xBF, 	0xC0, 	0xC1, 	0xC2, 	0xC3, 	0xC4, 	0xC5, 	0xC6, 	0xC7, 	0xC8, 	0xC9, 	0xCA, 	0xCB, 	0xCC, 	0xCD, 	0xCE, 	0xCF, 	0xD0, 	0xD1, 	0xD2, 	0xD3, 	0xD4, 	0xD5, 	0xD6, 	0xD7, 	0xD8, 	0xD9, 	0xDA, 	0xDB, 	0xDC, 	0xDD, 	0xE0, 	0xE1, 	0xE2, 	0xE3, 	0xE4, 	0xE5, 	0xE6, 	0xE7, 	0xFF};


Dongle::Dongle()
{
    currentl2w = NULL;
    state = STATE_DISCONNECTED;
    handle = NULL;
    activity = DO_NORMAL;

    for(int i = 0; i < KEYNUM; i++) key_transfer_state[i] = false;

    connect();
}

void Dongle::process()
{
    qDebug("Start checker");

    while(1){
        QTime updateTime;
        updateTime.start();
        int delta;


        if(state == STATE_DISCONNECTED){
            connect();
        }

        if(currentl2w != NULL){
            currentl2w->check();

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
                case DO_SETDONGLEON:
                    sendCMD_SET_STATE(true);
                    break;
                case DO_SETDONGLEOFF:
                    sendCMD_SET_STATE(false);
                    break;
                default:
                    break;
                }
                activity = DO_NORMAL;

                recieve_status() ;
                transmitHPCPMP();
            }
        }
        emit showStatus(state, updateTime.elapsed());
        delta = 100 - updateTime.elapsed();
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

    if (hid_get_input_report(handle, buf, sizeof(buf))==-1)
    {
      qDebug("Ошибка при  приёме данных");
      disconnect();
    } else {
        //qDebug("Recieved %d %d", buf[0], buf[1]);
        if(buf[1] == 0) {
            state = STATE_OFF;
        } else {
            state = STATE_ON;
        }
    }
}


int Dongle::connect()
{
    qDebug("Dongle::connect()");

    // Open the device using the VID, PID,
    // and optionally the Serial number.
    //handle = hid_open(0x03eb, 0x204d, L"12345");
    handle = hid_open(0x03eb, 0x204d, NULL);
    if (!handle) {
        qDebug("unable to open device\n");
        state = STATE_DISCONNECTED;
        return state;
    }
    // Set the hid_read() function to be non-blocking.
    hid_set_nonblocking(handle, 1);

    state = STATE_OFF;
    activity = DO_WRITEALLTODONGLE;
    return state;
}

int Dongle::disconnect()
{
    qDebug("Dongle::disconnect()");

    hid_close(handle);

    /* Free static HIDAPI objects. */
    hid_exit();
    state = STATE_DISCONNECTED;
    return STATE_DISCONNECTED;
}



int Dongle::send_command(int device, int command, unsigned char* cmd_arg)
{
    qDebug("Dongle::send_command(int device, int command, unsigned char* cmd_arg)command: %d", command);
    int res;
    unsigned char buf[HID_REPORT_SIZE];
    if(state == STATE_DISCONNECTED){
        connect();
    }
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
        res = hid_write(handle, buf, 9);
        if (res < 0) {
            qDebug("Unable to write()\n");
            qDebug("Error: %ls\n", hid_error(handle));
            return disconnect();
        }
        state = current_state;
    }
    return(state);
}




unsigned char Dongle::string2keycode(QString Key){
    for(int i = 0; i< 219; i++){
        if(strcmp(KeyMnemonicDB[i], Key.toStdString().c_str()) == 0) return KeyCodesDB[i];
    }
    return(0);
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
    qDebug("idHP: %d, idMP: %d, idMobHP: %d", hpbuf[1], hpbuf[3], hpbuf[0]);

    send_command(INTERFACE_ID_Keyboard, CMD_SET_HPCPMP, hpbuf);

}

void Dongle::sendCMD_SET_STATE(bool stt) {

    qDebug("Dongle::sendCMD_SET_STATE: %d", stt);

    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    send_command(INTERFACE_ID_GenericHID, stt, hpbuf);
}

void Dongle::sendCMD_WRITE_CONFIG(){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    send_command(INTERFACE_ID_Keyboard, CMD_WRITE_CONFIG, hpbuf);
}

void Dongle::sendCMD_READ_CONFIG(){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    send_command(INTERFACE_ID_Keyboard, CMD_READ_CONFIG, hpbuf);
}

void Dongle::sendCMD_ADD_NODE(QString Key, float PauseTime, float ReleaseTime, float ConditionTime){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));

   int p = (int)(PauseTime*2);  //; 0.5s grade
   int r = (int)(ReleaseTime*10);  //; 0.1s grade
   int c = (int)(ConditionTime*2); //; 0.5s grade

   p = (p>0xFF)? 0xFF: p;
   r = (r>0xFFFF)? 0xFFFF: r;
   c = (c>0xFF)? 0xFF: c;

    hpbuf[0] = string2keycode(Key);
    hpbuf[1] = p; // $xPauseTime
    hpbuf[2] = r & 0xFF; // BitAND(0xFF, $xReleaseTime)
    hpbuf[3] = (r >> 8) & 0xFF; // BitAND(0xFF, $xReleaseTime/256)
    hpbuf[4] = c; // $xConditionTime
    qDebug("CMD_ADD_NODE %s, Cooldown (0.1s) %d, Abs (0.5s) %d, Cond (0.5s) %d", Key.toStdString().c_str(), hpbuf[2]+hpbuf[3]*256, hpbuf[1], hpbuf[4]);
    send_command(INTERFACE_ID_Keyboard, CMD_ADD_NODE, hpbuf);
}

void Dongle::sendCMD_DELETE_NODE(QString Key){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));

    hpbuf[0] = string2keycode(Key);
    send_command(INTERFACE_ID_Keyboard, CMD_DELETE_NODE, hpbuf);
}


void Dongle::sendCMD_DELETE_ALL(){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    send_command(INTERFACE_ID_Keyboard, CMD_DELETE_ALL, hpbuf);
}


void Dongle::sendCMD_SET_MODIFIER(unsigned char Modifier){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));
    hpbuf[0] = Modifier;
    send_command(INTERFACE_ID_Keyboard, CMD_SET_MODIFIER, hpbuf);
}


void Dongle::sendCMD_ADD_NODE_CONDITION(QString Key, unsigned char Type, unsigned char Min, unsigned char Max){
    unsigned char hpbuf[HID_REPORT_SIZE-3];
    memset(hpbuf,0,sizeof(hpbuf));

    hpbuf[0] = string2keycode(Key);

//    Global Const $t_mobHP = 0 ;/** Mob HP */
//    Global Const $t_playerHP = 1 ;/** Player HP */
//    Global Const $t_playerCP = 2 ;/** Player CP */
//    Global Const $t_playerMP = 3 ;/** Player MP */

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
        hpbuf[1] = 4; //
        break;
    case idVP:
        hpbuf[1] = 5; //
        break;
    default:
        hpbuf[1] = 6; //
        break;
    }

    hpbuf[2] = (Min >=0 && Min <=100)? Min: 0xFF; //$Min
    hpbuf[3] = (Max >=0 && Max <=100)? Max: 0xFF;  // $Max

    send_command(INTERFACE_ID_Keyboard, CMD_ADD_NODE_CONDITION, hpbuf);
}

unsigned char Dongle::getXP(int idXP){
    if(currentl2w == NULL){
        return XP_ERR;
    } else {
        return currentl2w->getXP(idXP);
    }
}

void Dongle::sendKeyToDongle(int condition_index){
    qDebug("Dongle::sendKeyToDongle(int condition_index, bool state)");

    if(currentl2w == NULL)return;

     if(currentl2w->getCurrentSettings()->condition[condition_index]->FSet){
        sendCMD_ADD_NODE(
                     currentl2w->getCurrentSettings()->condition[condition_index]->KeyString,
                     currentl2w->getCurrentSettings()->condition[condition_index]->conditionf[idPause],
                     currentl2w->getCurrentSettings()->condition[condition_index]->conditionf[idCoolDown],
                     currentl2w->getCurrentSettings()->condition[condition_index]->conditionf[idCondition]);

        for(int j = idCP; j < BARNUM; j++ )
        {
            if(currentl2w->getCurrentSettings()->condition[condition_index]->conditioni[j] < 100 ||
               currentl2w->getCurrentSettings()->condition[condition_index]->conditioni[BARNUM+j] < 100 )
            {
                    sendCMD_ADD_NODE_CONDITION(
                                currentl2w->getCurrentSettings()->condition[condition_index]->KeyString,
                                j,
                                currentl2w->getCurrentSettings()->condition[condition_index]->conditioni[j],
                                currentl2w->getCurrentSettings()->condition[condition_index]->conditioni[BARNUM+j]
                                );
            }
        }
    } else {
        sendCMD_DELETE_NODE(currentl2w->getCurrentSettings()->condition[condition_index]->KeyString);
    }
    key_transfer_state[condition_index] = true;
}

void  Dongle::doSetState(bool stt){
    if(stt){
        activity = DO_SETDONGLEON;
    } else {
        activity = DO_SETDONGLEOFF;
    }
}

void Dongle::doSaveAllToDongle(){
    activity = DO_WRITEALLTODONGLE;
}

void Dongle::doSendKeyToDongle(int condition_index){
    qDebug("Dongle::doSendKeyToDongle(int condition_index)");
    key_transfer_state[condition_index] = false;
    activity = DO_WRITEKEYTODONGLE;

}
