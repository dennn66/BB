#ifndef L2WINDOW_H
#define L2WINDOW_H
#include <QString>
#include <QtWinExtras/QtWin>
#include <QImage>
#include <QIcon>
#include <QTextStream>
#include <QSettings>
#include <windows.h>

#include "xpbar.h"
#include "keycondition.h"
#include "keyconditionsset.h"



#define L2_OFF false
#define L2_ON true



class L2Window
{
public:
    L2Window(HWND winhwnd);
    bool init(QImage image);
    QIcon* getIcon();
    QString getTitle();
    QString getNic();
    HWND getHWND();
    QString project_file_name;

    int getStatus();
    int LoadProject(QString file_name);
    int SaveProject(QString file_name);
    int LoadConfig(QString file_name);
    int SaveConfig(QString file_name);
    int AddConfig(QString file_name);
    int getXP(int bar);
    int check();
    QString getConditionLabel(int index);
    bool getConditionState(int index);
    void findBars();
    bool isValidIndex(int index);
    bool activateSettings(int index);
    KeyConditionsSet* getCurrentSettings();
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
    int image_width;
    int image_height;
    //static char* DefaultKeyDB[KEYNUM];

    QImage capture();
    bool findXP(int index, QImage image);
    bool findCPHPMP(QImage image);

signals:

public slots:
};

#endif // L2WINDOW_H
