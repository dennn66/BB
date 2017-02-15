#include "keycondition.h"



const char* KeyCondition::KeyMnemonicDB[KEY_DB_SIZE] = {"ERROR_ROLLOVER", 	"POST_FAIL", 	"ERROR_UNDEFINED", 	"A", 	"B", 	"C", 	"D", 	"E", 	"F", 	"G", 	"H", 	"I", 	"J", 	"K", 	"L", 	"M", 	"N", 	"O", 	"P", 	"Q", 	"R", 	"S", 	"T", 	"U", 	"V", 	"W", 	"X", 	"Y", 	"Z", 	"1", 	"2", 	"3", 	"4", 	"5", 	"6", 	"7", 	"8", 	"9", 	"0", 	"ENTER", 	"ESCAPE", 	"BACKSPACE", 	"TAB", 	"SPACE", 	"-", 	"=", 	"[", 	"]", 	"\\", 	"`", 	";", 	"", 	"GRAVE_ACCENT_AND_TILDE", 	",", 	".", 	"/", 	"CAPS_LOCK", 	"F1", 	"F2", 	"F3", 	"F4", 	"F5", 	"F6", 	"F7", 	"F8", 	"F9", 	"F10", 	"F11", 	"F12", 	"PRINT_SCREEN", 	"SCROLL_LOCK", 	"PAUSE", 	"INSERT", 	"HOME", 	"PAGE_UP", 	"DELETE", 	"END", 	"PAGE_DOWN", 	"RIGHT_ARROW", 	"LEFT_ARROW", 	"DOWN_ARROW", 	"UP_ARROW", 	"NUM_LOCK", 	"KEYPAD_SLASH", 	"KEYPAD_ASTERISK", 	"KEYPAD_MINUS", 	"KEYPAD_PLUS", 	"KEYPAD_ENTER", 	"P1", 	"P2", 	"P3", 	"P4", 	"P5", 	"P6", 	"P7", 	"P8", 	"P9", 	"P0", 	"KEYPAD_DOT_AND_DELETE", 	"NON_US_BACKSLASH_AND_PIPE", 	"APPLICATION", 	"POWER", 	"KEYPAD_EQUAL_SIGN", 	"F13", 	"F14", 	"F15", 	"F16", 	"F17", 	"F18", 	"F19", 	"F20", 	"F21", 	"F22", 	"F23", 	"F24", 	"EXECUTE", 	"HELP", 	"MANU", 	"SELECT", 	"STOP", 	"AGAIN", 	"UNDO", 	"CUT", 	"COPY", 	"PASTE", 	"FIND", 	"MUTE", 	"VOLUME_UP", 	"VOLUME_DOWN", 	"LOCKING_CAPS_LOCK", 	"LOCKING_NUM_LOCK", 	"LOCKING_SCROLL_LOCK", 	"KEYPAD_COMMA", 	"KEYPAD_EQUAL_SIGN_AS400", 	"INTERNATIONAL1", 	"INTERNATIONAL2", 	"INTERNATIONAL3", 	"INTERNATIONAL4", 	"INTERNATIONAL5", 	"INTERNATIONAL6", 	"INTERNATIONAL7", 	"INTERNATIONAL8", 	"INTERNATIONAL9", 	"LANG1", 	"LANG2", 	"LANG3", 	"LANG4", 	"LANG5", 	"LANG6", 	"LANG7", 	"LANG8", 	"LANG9", 	"ALTERNATE_ERASE", 	"SISREQ", 	"CANCEL", 	"CLEAR", 	"PRIOR", 	"RETURN", 	"SEPARATOR", 	"OUT", 	"OPER", 	"CLEAR_AND_AGAIN", 	"CRSEL_ANDPROPS", 	"EXSEL", 	"KEYPAD_00", 	"KEYPAD_000", 	"THOUSANDS_SEPARATOR", 	"DECIMAL_SEPARATOR", 	"CURRENCY_UNIT", 	"CURRENCY_SUB_UNIT", 	"KEYPAD_OPENING_PARENTHESIS", 	"KEYPAD_CLOSING_PARENTHESIS", 	"KEYPAD_OPENING_BRACE", 	"KEYPAD_CLOSING_BRACE", 	"KEYPAD_TAB", 	"KEYPAD_BACKSPACE", 	"KEYPAD_A", 	"KEYPAD_B", 	"KEYPAD_C", 	"KEYPAD_D", 	"KEYPAD_E", 	"KEYPAD_F", 	"KEYPAD_XOR", 	"KEYPAD_CARET", 	"KEYPAD_PERCENTAGE", 	"KEYPAD_LESS_THAN_SIGN", 	"KEYPAD_GREATER_THAN_SIGN", 	"KEYPAD_AMP", 	"KEYPAD_AMP_AMP", 	"KEYPAD_PIPE", 	"KEYPAD_PIPE_PIPE", 	"KEYPAD_COLON", 	"KEYPAD_HASHMARK", 	"KEYPAD_SPACE", 	"KEYPAD_AT", 	"KEYPAD_EXCLAMATION_SIGN", 	"KEYPAD_MEMORY_STORE", 	"KEYPAD_MEMORY_RECALL", 	"KEYPAD_MEMORY_CLEAR", 	"KEYPAD_MEMORY_ADD", 	"KEYPAD_MEMORY_SUBTRACT", 	"KEYPAD_MEMORY_MULTIPLY", 	"KEYPAD_MEMORY_DIVIDE", 	"KEYPAD_PLUS_AND_MINUS", 	"KEYPAD_CLEAR", 	"KEYPAD_CLEAR_ENTRY", 	"KEYPAD_BINARY", 	"KEYPAD_OCTAL", 	"KEYPAD_DECIMAL", 	"KEYPAD_HEXADECIMAL", 	"LEFT_CONTROL", 	"LEFT_SHIFT", 	"LEFT_ALT", 	"LEFT_GUI", 	"RIGHT_CONTROL", 	"RIGHT_SHIFT", 	"RIGHT_ALT", 	"RIGHT_GUI",
"MOUSE_LEFT", "MOUSE_RIGHT", "MOUSE_MIDDLE", "MOUSE_4th", "MOUSE_5th"};
unsigned char KeyCondition::KeyCodesDB[KEY_DB_SIZE] = {0x01, 	0x02, 	0x03, 	0x04, 	0x05, 	0x06, 	0x07, 	0x08, 	0x09, 	0x0A, 	0x0B, 	0x0C, 	0x0D, 	0x0E, 	0x0F, 	0x10, 	0x11, 	0x12, 	0x13, 	0x14, 	0x15, 	0x16, 	0x17, 	0x18, 	0x19, 	0x1A, 	0x1B, 	0x1C, 	0x1D, 	0x1E, 	0x1F, 	0x20, 	0x21, 	0x22, 	0x23, 	0x24, 	0x25, 	0x26, 	0x27, 	0x28, 	0x29, 	0x2A, 	0x2B, 	0x2C, 	0x2D, 	0x2E, 	0x2F, 	0x30, 	0x31, 	0x32, 	0x33, 	0x34, 	0x35, 	0x36, 	0x37, 	0x38, 	0x39, 	0x3A, 	0x3B, 	0x3C, 	0x3D, 	0x3E, 	0x3F, 	0x40, 	0x41, 	0x42, 	0x43, 	0x44, 	0x45, 	0x46, 	0x47, 	0x48, 	0x49, 	0x4A, 	0x4B, 	0x4C, 	0x4D, 	0x4E, 	0x4F, 	0x50, 	0x51, 	0x52, 	0x53, 	0x54, 	0x55, 	0x56, 	0x57, 	0x58, 	0x59, 	0x5A, 	0x5B, 	0x5C, 	0x5D, 	0x5E, 	0x5F, 	0x60, 	0x61, 	0x62, 	0x63, 	0x64, 	0x65, 	0x66, 	0x67, 	0x68, 	0x69, 	0x6A, 	0x6B, 	0x6C, 	0x6D, 	0x6E, 	0x6F, 	0x70, 	0x71, 	0x72, 	0x73, 	0x74, 	0x75, 	0x76, 	0x77, 	0x78, 	0x79, 	0x7A, 	0x7B, 	0x7C, 	0x7D, 	0x7E, 	0x7F, 	0x80, 	0x81, 	0x82, 	0x83, 	0x84, 	0x85, 	0x86, 	0x87, 	0x88, 	0x89, 	0x8A, 	0x8B, 	0x8C, 	0x8D, 	0x8E, 	0x8F, 	0x90, 	0x91, 	0x92, 	0x93, 	0x94, 	0x95, 	0x96, 	0x97, 	0x98, 	0x99, 	0x9A, 	0x9B, 	0x9C, 	0x9D, 	0x9E, 	0x9F, 	0xA0, 	0xA1, 	0xA2, 	0xA3, 	0xA4, 	0xB0, 	0xB1, 	0xB2, 	0xB3, 	0xB4, 	0xB5, 	0xB6, 	0xB7, 	0xB8, 	0xB9, 	0xBA, 	0xBB, 	0xBC, 	0xBD, 	0xBE, 	0xBF, 	0xC0, 	0xC1, 	0xC2, 	0xC3, 	0xC4, 	0xC5, 	0xC6, 	0xC7, 	0xC8, 	0xC9, 	0xCA, 	0xCB, 	0xCC, 	0xCD, 	0xCE, 	0xCF, 	0xD0, 	0xD1, 	0xD2, 	0xD3, 	0xD4, 	0xD5, 	0xD6, 	0xD7, 	0xD8, 	0xD9, 	0xDA, 	0xDB, 	0xDC, 	0xDD, 	0xE0, 	0xE1, 	0xE2, 	0xE3, 	0xE4, 	0xE5, 	0xE6, 	0xE7,
                                                 0xF0, 	0xF1, 	0xF2, 	0xF3, 	0xF4};

const char* KeyCondition::conditionf_tag[CONDFNUM] = {
    "$FCoolDownSet",
    "$FPauseSet",
    "$FConditionIdleSet"
};

const char* KeyCondition::conditioni_tag[CONDINUM] = {
    "$FCPMinSet",
    "$FHPMinSet",
    "$FMPMinSet",
    "$FVPMinSet",
    "$FMobHPMinSet",
    "$FMobMPMinSet",
    "$FCPMaxSet",
    "$FHPMaxSet",
    "$FMPMaxSet",
    "$FVPMaxSet",
    "$FMobHPMaxSet",
    "$FMobMPMaxSet"
};

const char* KeyCondition::conditionb_tag[CONDBNUM] = {
    "$FGroupEnable0",
    "$FGroupEnable1",
    "$FGroupEnable2",
    "$FGroupEnable3",
    "$FCheckStar",
    "$FStarState",
    "$FTargetMeOrPet",
    "$FTargetChar",
    "$FTargetMob",
    "$FNoTarget",
    "$FCheckSkill",
    "$FCheckPet",
    "$FPetState",

    "$FNotUseSkill1",
    "$FNotUseSkill2",
    "$FNotUseSkill3",
    "$FNotUseSkill4",

    "$FNotUseSkill5",
    "$FNotUseSkill6",
    "$FNotUseSkill7",
    "$FNotUseSkill8",

    "$FNotUseSkill9",
    "$FNotUseSkill10",
    "$FNotUseSkill11",
    "$FNotUseSkill12",

    "$FNotUseSkill13",
    "$FNotUseSkill14",
    "$FNotUseSkill15",
    "$FNotUseSkill16",

    "$FNotUseSkill17",
    "$FNotUseSkill18",
    "$FNotUseSkill19",
    "$FNotUseSkill20",

    "$FNotUseSkill21",
    "$FNotUseSkill22",
    "$FNotUseSkill23",
    "$FNotUseSkill24",

    "$FNotUseSkill25",
    "$FNotUseSkill26",
    "$FNotUseSkill27",
    "$FNotUseSkill28",

    "$FNotUseSkill29",
    "$FNotUseSkill30",
    "$FNotUseSkill31",
    "$FNotUseSkill32",

    "$FNotUseSkill33",
    "$FNotUseSkill34",
    "$FNotUseSkill35",
    "$FNotUseSkill36",

    "$FNotUseSkill37",
    "$FNotUseSkill38",
    "$FNotUseSkill39",
    "$FNotUseSkill40",

    "$FNotUseSkill41",
    "$FNotUseSkill42",
    "$FNotUseSkill43",
    "$FNotUseSkill44",

    "$FNotUseSkill45",
    "$FNotUseSkill46",
    "$FNotUseSkill47",
    "$FNotUseSkill48"
};

const char* KeyCondition::conditionb_name[CONDBNUM] = {
    "B0",
    "B1",
    "B2",
    "B3",
    "CheckStar",
    "StarState",
    "Me or Pet",
    "Char or NPC",
    "Mob",
    "No Target",
    "Check Skill",
    "Check Pet",
    "Pet State",
    "$FNotUseSkill1",
    "$FNotUseSkill2",
    "$FNotUseSkill3",
    "$FNotUseSkill4",

    "$FNotUseSkill5",
    "$FNotUseSkill6",
    "$FNotUseSkill7",
    "$FNotUseSkill8",

    "$FNotUseSkill9",
    "$FNotUseSkill10",
    "$FNotUseSkill11",
    "$FNotUseSkill12",

    "$FNotUseSkill13",
    "$FNotUseSkill14",
    "$FNotUseSkill15",
    "$FNotUseSkill16",

    "$FNotUseSkill17",
    "$FNotUseSkill18",
    "$FNotUseSkill19",
    "$FNotUseSkill20",

    "$FNotUseSkill21",
    "$FNotUseSkill22",
    "$FNotUseSkill23",
    "$FNotUseSkill24",

    "$FNotUseSkill25",
    "$FNotUseSkill26",
    "$FNotUseSkill27",
    "$FNotUseSkill28",

    "$FNotUseSkill29",
    "$FNotUseSkill30",
    "$FNotUseSkill31",
    "$FNotUseSkill32",

    "$FNotUseSkill33",
    "$FNotUseSkill34",
    "$FNotUseSkill35",
    "$FNotUseSkill36",

    "$FNotUseSkill37",
    "$FNotUseSkill38",
    "$FNotUseSkill39",
    "$FNotUseSkill40",

    "$FNotUseSkill41",
    "$FNotUseSkill42",
    "$FNotUseSkill43",
    "$FNotUseSkill44",

    "$FNotUseSkill45",
    "$FNotUseSkill46",
    "$FNotUseSkill47",
    "$FNotUseSkill48"
};

KeyCondition::KeyCondition(QString button)
{
    FSet = false;
    ctrl = false;
    shift = false;
    KeyString = button;
    KeyCode = string2keycode(KeyString);
    conditionf[idCoolDown] = 1;
    conditionf[idPause] = 0;
    conditionf[idCondition] = 0;

    for(int i = 0; i<CONDINUM; i++){
        conditioni[i] = 0xFF;
    }

    for(int i = 0; i<CONDBNUM; i++){
        conditionb[i] = 0;
    }

}

void KeyCondition::setGroupState(int group_number, bool state){
    qDebug("KeyConditionsSet::setGroupState(int group, bool state)");
    conditionb[group_number+idGroupB1] = state;
}

bool KeyCondition::getGroupState(int group_number){
    qDebug("KeyConditionsSet::getGroupState(int group, bool state)");
    return(conditionb[group_number+idGroupB1]);
}


unsigned char KeyCondition::getTargetTypeBinaryCondition(){
    qDebug("KeyCondition::getTargetTypeBinaryState()");
    unsigned char state = 0;

    for(int i = idTargetMeOrPet; i < idNoTarget+1; i++){
        state |= conditionb[i] << (i-idTargetMeOrPet);
    }
    state &= 0xFF;
    return(state);
}

unsigned char KeyCondition::getStarStatusBinaryCondition(){
    qDebug("KeyCondition::getStarStatusBinaryState()");
    unsigned char state = 0;

    for(int i = idCheckStar; i<idStarCondition+1; i++){
        state |= conditionb[i] << (1-(i-idCheckStar));
    }
    state &= 0xFF;
    return(state);
}

unsigned char KeyCondition::getGroupsBinaryCondition(){
    qDebug("KeyCondition::getGroupState()");
    unsigned char state = 0;
    for(int i = idGroupB1; i<idGroupB4+1; i++){
        state |= conditionb[i] << (i);
    }
    state &= 0xFF;
    return(state);
}

KeyCondition::~KeyCondition()
{
    qDebug("KeyCondition::~KeyCondition()");

}



bool KeyCondition::getConditionB(int index){
    if(index >= 0 && index < CONDBNUM){
        return conditionb[index];
    }
    return false;
}

void KeyCondition::setConditionB(int index, bool b){
    if(index >= 0 && index < CONDBNUM){
        conditionb[index] = b;
    }
}

unsigned char KeyCondition::string2keycode(QString Key){
    for(int i = 0; i< KEY_DB_SIZE; i++){
        if(strcmp(KeyMnemonicDB[i], Key.toStdString().c_str()) == 0) return KeyCodesDB[i];
    }
    return(0);
}
