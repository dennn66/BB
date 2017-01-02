#ifndef L2WINDOW_H
#define L2WINDOW_H
#include <QString>
#include <QtWinExtras/QtWin>
#include <QImage>
#include <QIcon>
#include <QTextStream>
#include <QSettings>
#include <QMessageBox>
#include <windows.h>

#include "xpbar.h"
#include "keycondition.h"
#include "keyconditionsset.h"



#define L2_OFF false
#define L2_ON true

#define TARGETMEORPET   0
#define TARGETCHAR      1
#define TARGETMOB       2
#define NOTARGET        3
#define TARGETSTATESNUM 4

class L2Window
{
public:
    int targettype;
    L2Window(HWND winhwnd);
    bool init(QImage image);
    QIcon* getIcon();
    bool getStarState();
    QColor* getStarColor();
    QString getTitle();
    QString getNic();
    HWND getHWND();
    QString project_file_name;
    QColor c2; //token
    QPoint windowtopright;
    bool isActiveWindow;

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
    void resetBars();
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
    bool bStar;
    int image_width;
    int image_height;
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
    QImage star;
    QPoint startopleft;

    QPoint findPattern(QImage source, QPoint topleft, QPoint bottomright, QImage pattern, int delta);

    //static char* DefaultKeyDB[KEYNUM];

    QImage capture();
//    bool findXP(int index, QImage image);
    bool findCPHPMP(QImage image);

signals:

public slots:
};

#endif // L2WINDOW_H
