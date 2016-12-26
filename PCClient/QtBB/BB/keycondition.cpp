#include "keycondition.h"

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
    "$FNoTarget"
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
    "No Target"
};

KeyCondition::KeyCondition(QString button)
{
    FSet = false;
    ctrl = false;
    shift = false;
    KeyString = button;
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


unsigned char KeyCondition::getTargetTypeBinaryState(){
    qDebug("KeyCondition::getTargetTypeBinaryState()");
    unsigned char state = 0;

    for(int i = idTargetMeOrPet; i < idNoTarget+1; i++){
        state |= conditionb[i] << (i-idTargetMeOrPet);
    }
    state &= 0xFF;
    return(state);
}

unsigned char KeyCondition::getStarStatusBinaryState(){
    qDebug("KeyCondition::getStarStatusBinaryState()");
    unsigned char state = 0;

    for(int i = idCheckStar; i<idStarState+1; i++){
        state |= conditionb[i] << (1-(i-idCheckStar));
    }
    state &= 0xFF;
    return(state);
}

unsigned char KeyCondition::getGroupsBinaryState(){
    qDebug("KeyCondition::getGroupState()");
    unsigned char state = 0;
    for(int i = idGroupB1; i<idGroupB4+1; i++){
        state |= conditionb[i] << (i+2);
    }
    state &= 0xFF;
    return(state);
}

KeyCondition::~KeyCondition()
{
    qDebug("KeyCondition::~KeyCondition()");

}
