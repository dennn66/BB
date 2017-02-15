#ifndef KEYCONDITION_H
#define KEYCONDITION_H
#include <QString>
#include "xpbar.h"


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
#define idStarCondition       5
#define idTargetMeOrPet   6
#define idTargetChar      7
#define idTargetMob       8
#define idNoTarget        9
#define idCheckSkillTimeout      10
#define idCheckPet       11
#define idPetState       12
#define idNoUseSkillState     13
#define idNoUseSkillNum       48

#define TARGETMEORPET   0
#define TARGETCHAR      1
#define TARGETMOB       2
#define NOTARGET        3
#define TARGETSTATESNUM 4

#define CONDBNUM 13+48

#define GROUPSNUM 4
#define TARGETTYPENUM 4

#define KEY_DB_SIZE         223

class KeyCondition
{
public:
    KeyCondition(QString button);
    ~KeyCondition();

    static const char* KeyMnemonicDB[KEY_DB_SIZE];
    static unsigned char KeyCodesDB[KEY_DB_SIZE];
    unsigned char string2keycode(QString Key);

    void setGroupState(int group, bool state);
    bool getGroupState(int group);
    unsigned char getGroupsBinaryCondition();
    unsigned char getTargetTypeBinaryCondition();
    unsigned char getStarStatusBinaryCondition();
    bool getStarCondition(){ return conditionb[idStarCondition];}
    bool getCheckStar(){ return conditionb[idCheckStar];}
    bool checkStarCondition(bool star){
        if(conditionb[idCheckStar]){
            return (conditionb[idStarCondition] == star);
        } else {
            return true;
        }
    }
    bool checkTargetCondition(unsigned char target){ return conditionb[idTargetMeOrPet+target];}

    bool checkBarCondition(int num, unsigned char bar, unsigned char target){

        if(!((num == idMinMobHP) && (target != TARGETMOB))){
            if(!((num == idMinMobMP) && (target != TARGETMEORPET))){
                if(bar > 100) return false;
                if((conditioni[num] < 101) && (conditioni[num] > bar))  return false;
                if((conditioni[BARNUM+num] < 101) && (conditioni[BARNUM+num] < bar))  return false;
            }
        }
        return true;
        if(num <  BARNUM && bar <= 100 )
        {
            if(conditioni[num] <= 100 && bar <  conditioni[num]) return false;
            if(conditioni[BARNUM+num] <= 100 && bar >  conditioni[BARNUM+num]) return false;
        }
        return true;
    }

    bool FSet;
    float conditionf[CONDFNUM]; //Cooldown, Pause, ConditionIdle
    unsigned char  conditioni[CONDINUM]; //Min Max
    QString KeyString;
    unsigned char KeyCode;
    bool ctrl;
    bool shift;


    static const char* conditionf_tag[CONDFNUM];
    static const char* conditioni_tag[CONDINUM];
    static const char* conditionb_tag[CONDBNUM];
    static const char* conditionb_name[CONDBNUM];
    void setConditionB(int index, bool b);
    bool getConditionB(int index);

private:
    bool conditionb[CONDBNUM];

};

#endif // KEYCONDITION_H
