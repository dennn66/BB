#ifndef KEYCONDITION_H
#define KEYCONDITION_H
#include <QString>



#define idCoolDown      0
#define idPause         1
#define idCondition     2
#define CONDFNUM 3

#define idMinCP         0
#define idMinHP         1
#define idMinMP         2
#define idMinVP         3
#define idMinMobHP      4
#define idMinMobMP      5
#define idMaxCP         6
#define idMaxHP         7
#define idMaxMP         8
#define idMaxVP         9
#define idMaxMobHP      10
#define idMaxMobMP      11
#define CONDINUM 12


class KeyCondition
{
public:
    KeyCondition(QString button);
    ~KeyCondition();
    bool FSet;
    float conditionf[CONDFNUM]; //Cooldown, Pause, ConditionIdle
    unsigned char  conditioni[CONDINUM]; //Min Max
    QString KeyString;

    static char* conditionf_tag[CONDFNUM];
    static char* conditioni_tag[CONDINUM];

};

#endif // KEYCONDITION_H
