#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWinExtras/QtWin>
#include <QMessageBox>
#include <QTextStream>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QThread>
#include <QtMultimedia/QSound>
#include <QtMultimedia/QMediaPlayer>
#include <QProgressBar>
#include <QGraphicsScene>
#include <QKeyEvent>
#include "SystemKeyboardReadWrite.h"
#include "l2window.h"
#include "dongle.h"
#include "dongleworker.h"
#include "l2parser.h"
#include "keysettingsdialog.h"
#include "clicker.h"

#define K_DISABLE 0
#define K_ENABLE 1
#define K_TOGGLE 2
#define K_NUM 3

#define B_ONOFF 0
#define B_B1    1
#define B_B2    2
#define B_B3    3
#define B_B4    4
#define B_NUM   5

#define VK_NUM K_NUM*B_NUM



BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam);

namespace Ui {
class BoredomBreaker;
}

class BoredomBreaker : public QMainWindow
{
    Q_OBJECT

public:
    explicit BoredomBreaker(QWidget *parent = 0);
    ~BoredomBreaker();
    void addL2Window(HWND hwnd);
private:
    Ui::BoredomBreaker *ui;
    QVector <L2Window*> l2list;

    QLabel *keylabel[KEYNUM];
    QCheckBox *keyenable[KEYNUM];
    QCheckBox *keyenable2[GROUPSNUM];
    QPushButton *keysettings[KEYNUM];
//    QThread* dongle_thread;
    //Dongle* dongle;
    DongleWorker* dongle_worker;
    Clicker* clicker;

    QThread* l2_parser_thread;
    L2parser* l2_parser;


//    bool group_enable[CONDBNUM];
    QProgressBar *pb[BARNUM];
    SystemKeyboardReadWrite *kb;

    bool bModifier[VK_NUM];
    bool bEnableSound;
//    bool bEnableModifier;
    DWORD vkModifierCode[VK_NUM];
    DWORD vkActivationKeyCode[VK_NUM];
    QString default_file_name;

    int ellipsed_time;
    static const char* StyleSheet[BARNUM+1];
    static const char* StyleSheetCheckBox[5];
    static const char* StyleSheetLabel[2];

    void enumerateL2();
    bool isValidIndex(int index);
    void enableGroup(int group, bool state);


public slots:
    void showDongleStatus(unsigned char d_stt, unsigned char g_stt, int updatetime); /* */
    void showParserStatus(int updatetime,  bool mainleftstatus, bool mainrightstatus, bool mobleftstatus,  bool mobrightstatus, bool toolbarstatus, bool getpetstatus, int target);
    void cbDongleClicked(bool checked);
    void cbCtrlShiftClicked(bool checked);
    void cbKeyEnableClicked(bool checked);
    void cbKeyEnableBxClicked(bool checked);
    void pbLoadClicked();
    void pbSaveProjectClicked();
    void pbLoadProjectClicked();
    void pbSaveClicked();
    void pbAddClicked();
    void pbToDongleClicked();
    void pbJumpToBootloaderClicked();
    void pbKeySettingsClicked();
    void pbEnumerateClicked();
    void pbFindBarsClicked();
    void cmbWinListActivated(int index);
    void cmbCondSetListActivated(int index);
    void cmbCondSetListTextChanged(const QString &text);
    void keyGlobalPressed(DWORD vkCode);
    void keyGlobalReleased(DWORD vkCode);
    void doActivateL2();
    void pbSettingsClicked();

signals:
    //void setDongleState(int stt);
    void setDongleGroupState(int i, bool state);
    void doSetState(bool stt);
    void doSetModifier(bool bCtrl, bool bShift);
    void doSendKeyToDongle(int condition_index);
    void doSaveAllToDongle();
    void doJumpToBootloader();
    void setActiveL2W(L2Window* l2w);
};

#endif // MAINWINDOW_H
