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
    KeyString = button;
    conditionf[idCoolDown] = 1;
    conditionf[idPause] = 0;
    conditionf[idCondition] = 0;

    for(int i = 0; i<CONDINUM; i++){
        conditioni[i] = 0xFF;
    }

}

KeyCondition::~KeyCondition()
{
    qDebug("KeyCondition::~KeyCondition()");

}
