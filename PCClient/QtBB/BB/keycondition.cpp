#include "keycondition.h"

char* KeyCondition::conditionf_tag[CONDFNUM] = {
    "$FCoolDownSet",
    "$FPauseSet",
    "$FConditionIdleSet"
};

char* KeyCondition::conditioni_tag[CONDINUM] = {
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
        group_enable[i] = 0;
    }

}

void KeyCondition::setGroupState(int group_number, bool state){
    qDebug("KeyConditionsSet::setGroupState(int group, bool state)");
    group_enable[group_number] = state;
}

bool KeyCondition::getGroupState(int group_number){
    qDebug("KeyConditionsSet::getGroupState(int group, bool state)");
    return(group_enable[group_number]);
}


unsigned char KeyCondition::getGroupsBinaryState(){
    qDebug("KeyCondition::getGroupState()");
    unsigned char state = 0;
    for(int i = 0; i<CONDBNUM; i++){
        state |= group_enable[i] << (i+2);
    }
    state &= 0xFF;
    return(state);
}

KeyCondition::~KeyCondition()
{
    qDebug("KeyCondition::~KeyCondition()");

}
