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
#include "l2parser.h"
#include "keysettingsdialog.h"

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
    QThread* dongle_thread;
    Dongle* dongle;

    QThread* l2_parser_thread;
    L2parser* l2_parser;


    bool group_enable[CONDBNUM];
    QProgressBar *pb[BARNUM];
    SystemKeyboardReadWrite *kb;
    bool bModifier;
    bool bEnableSound;
    bool bEnableModifier;
    DWORD vkModifierCode;
    DWORD vkActivationKeyCode;
    QString default_file_name;

    int ellipsed_time;
    static char* StyleSheet[BARNUM+1];
    static char* StyleSheetCheckBox[5];
    static char* StyleSheetLabel[2];

    void enumerateL2();
    bool isValidIndex(int index);
    void toggleGroup(int group);
    void enableGroup(int group, bool state);
    unsigned char getGroupsBinaryState();
    bool getGroupBoolState(int group_num, unsigned char bin_state);



public slots:
    void showDongleStatus(int state, int g_state, int updatetime); /* */
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

signals:
    void setDongleState(int stt);
};

#endif // MAINWINDOW_H
