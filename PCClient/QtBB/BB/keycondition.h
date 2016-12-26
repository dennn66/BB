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

#define idGroupB1         0
#define idGroupB2         1
#define idGroupB3         2
#define idGroupB4         3
#define idCheckStar       4
#define idStarState       5
#define idTargetMeOrPet   6
#define idTargetChar      7
#define idTargetMob       8
#define idNoTarget        9



#define CONDBNUM 10
#define GROUPSNUM 4
#define TARGETTYPENUM 4

class KeyCondition
{
public:
    KeyCondition(QString button);
    ~KeyCondition();
    void setGroupState(int group, bool state);
    bool getGroupState(int group);
    unsigned char getGroupsBinaryState();
    unsigned char getTargetTypeBinaryState();
    unsigned char getStarStatusBinaryState();

    bool FSet;
    float conditionf[CONDFNUM]; //Cooldown, Pause, ConditionIdle
    unsigned char  conditioni[CONDINUM]; //Min Max
    QString KeyString;
    bool conditionb[CONDBNUM];
    bool ctrl;
    bool shift;


    static const char* conditionf_tag[CONDFNUM];
    static const char* conditioni_tag[CONDINUM];
    static const char* conditionb_tag[CONDBNUM];
    static const char* conditionb_name[CONDBNUM];

};

#endif // KEYCONDITION_H
