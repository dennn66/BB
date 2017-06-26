#ifndef L2WINDOW_H
#define L2WINDOW_H
#include <QString>
#include <QtWinExtras/QtWin>
#include <QImage>
#include <QIcon>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QSettings>
#include <QMessageBox>
#include <windows.h>

#include "xpbar.h"
#include "keycondition.h"
#include "keyconditionsset.h"



#define L2_OFF false
#define L2_ON true

#define TOOLLOFFSET 38
#define TOOLTOFFSET 9
#define TOOLVGAP 14
#define TOOLHGAP 5
#define TOOLH2GAP 7
#define TOOLSELL 32

class L2Window
{
public:
    int targettype;
    L2Window(HWND winhwnd);
    bool init(QImage image);
    QIcon* getIcon();
    int getTokenState();
    QColor* getTokenColor();
    QString getTitle();
    QString getNic();
    HWND getHWND();
    QString project_file_name;
    QColor token_color; //token
    QPoint windowtopright;
    bool isActiveWindow;

    int LoadProject(QString file_name);
    int SaveProject(QString file_name);
    int LoadConfig(QString file_name);
    int SaveConfig(QString file_name);
    int AddConfig(QString file_name);
    int getXP(int bar);
    int check();
    QImage* getTool(int n);
    QImage* findTool(int n);
    QImage image;
    QString getConditionLabel(int index);
    bool getConditionState(int index);
    void resetBars();
    bool isValidIndex(int index);
    bool activateSettings(int index);
    KeyConditionsSet* getCurrentSettings();
    bool isSkillRdy(int num);
    bool getConditionSkill(int index){return getCurrentSettings()->condition[index]->getConditionB(idCheckSkillTimeout);}
    bool getMainLeftStatus(){return (maintopleft.rx() > 0);}
    bool getMainRightStatus(){return (maintopright.rx() > 0);}
    bool getMobLeftStatus(){return (mobtopleft.rx() > 0);}
    bool getMobRightStatus(){return (mobtopright.rx() > 0);}
    bool getToolbarStatus(){return (toolbartopleft.rx() > 0);}
    bool getPetStatus(){return (pettopleft.rx() > 0);}
    int  getTargetType(){return targettype;}
//    bool getGroupState(int gr){return group_state[gr];}
//    void setGroupState(int gr, bool stt){group_state[gr] = stt;}
    void getBarStatistic();

    //KeyCondition* condition[KEYNUM];

    int activeCondSet;
    //KeyConditionsSet* conditionSet;
    QVector <KeyConditionsSet*> cond_set_list;


private:
    HWND hwnd;
    bool status;
    XPBar bar[BARNUM];
    //QString nic;
    QIcon* L2icon;
    int nToken;
    bool bEnableToolbar;
    int image_width;
    int image_height;
    bool skillrdy[KEYNUM];
//    bool group_state[GROUPSNUM];

    QImage mainleft;
    QImage mainright;
    QPoint maintopleft;
    QPoint maintopright;

    QImage mobleft_o;
    QImage mobright_o;
    QImage mobleft_c;
    QImage mobright_c;
    QPoint mobtopleft;
    QPoint mobtopright;
    int mobdetectcounter;

    QImage star;
    QPoint startopleft;
    int stardetectcounter;

    QImage toolbarleft;
    QPoint toolbartopleft;
    QImage tool[KEYNUM];
    int tooldetectcounter;

    QImage petleft;
    QPoint pettopleft;
    int petdetectcounter;
    bool bPetDetected;
    bool bPet;

    QImage mobhp;
    QImage mobmp;

    QPoint findPattern(QImage source, QPoint topleft, QPoint bottomright, QImage pattern, int delta);

    QImage capture();
    bool findCPHPMP(QImage image);

signals:

public slots:
};

#endif // L2WINDOW_H
