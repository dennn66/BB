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

    bool bModifier;
    bool bEnableSound;
    bool bEnableModifier;
    DWORD vkModifierCode;
    DWORD vkActivationKeyCode;
    QString default_file_name;

    int ellipsed_time;
    static const char* StyleSheet[BARNUM+1];
    static const char* StyleSheetCheckBox[5];
    static const char* StyleSheetLabel[2];

    void enumerateL2();
    bool isValidIndex(int index);
    void enableGroup(int group, bool state);


public slots:
    void showDongleStatus(unsigned char d_stt, int updatetime); /* */
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
    void pbKeySettingsClicked();
    void pbEnumerateClicked();
    void pbFindBarsClicked();
    void cmbWinListActivated(int index);
    void cmbCondSetListActivated(int index);
    void cmbCondSetListTextChanged(const QString &text);
    void keyGlobalPressed(DWORD vkCode);
    void keyGlobalReleased(DWORD vkCode);
    void doActivateL2();

signals:
    //void setDongleState(int stt);
    void setDongleGroupState(int i, bool state);
    void doSetState(bool stt);
    void doSetModifier(bool bCtrl, bool bShift);
    void doSendKeyToDongle(int condition_index);
    void doSaveAllToDongle();
    void setActiveL2W(L2Window* l2w);
};

#endif // MAINWINDOW_H
