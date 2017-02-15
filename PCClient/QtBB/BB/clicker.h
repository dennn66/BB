#ifndef CLICKER_H
#define CLICKER_H

#include <QWidget>
#include <QtGui>
#include <QVBoxLayout>
#include "keysettingsdialog.h"
#include "dongle.h"
#include "SystemMouseHook.h"


namespace Ui {
class Clicker;
}

class Clicker : public QWidget
{
    Q_OBJECT

public:
    explicit Clicker(QWidget *parent = 0);
    //Dongle* dongle;
    ~Clicker();
protected:

private:
    Ui::Clicker *ui;


   // bool group_enable[CONDBNUM];
    QCheckBox *keyenable2[GROUPSNUM];
    void toggleGroup(int group);
    void enableGroup(int group, bool state);
    bool isUnderWidget(QWidget* widget, int x, int y);
    void drawStatusBtn(QImage* imgStatus, bool pressed,bool mainleftstatus, bool mainrightstatus, bool mobleftstatus,  bool mobrightstatus, bool toolbarstatus, bool petstatus, int target);
    static const char* StyleSheetCheckBox[5];
    SystemMouseHook *ms;
    int right_offset;
    int top_offset;
    QImage* released_btn;
    QImage* pressed_btn;
    QImage* green_frame;
    QImage* red_frame;
    bool bFindBarsIsPressed;
    bool bSettingsIsPressed;



public slots:
    void cbDongleClicked(bool checked);
    void cbCtrlShiftClicked(bool checked);
    void cbKeyEnableBxClicked(bool checked);
    void showDongleStatus(unsigned char d_stt, int updatetime); /* */
    void showParserStatus(int updatetime, bool mainleftstatus, bool mainrightstatus, bool mobleftstatus,  bool mobrightstatus, bool toolbarstatus, bool petstatus, int target);
    void isL2Active(bool isActive, int right, int top);
    // Broadcasts a key has been pressed
    void keyLPressed(int x, int y);
    // Broadcasts a key has been released
    void keyLReleased(int x, int y);


signals:
    void setDongleGroupState(int i, bool state);
    void doSetState(bool stt);
    void doSetModifier(bool bCtrl, bool bShift);
    void doActivateL2();
    void pbFindBarsClicked();
    void pbSettingsClicked();
};

#endif // CLICKER_H
