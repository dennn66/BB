#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWinExtras/QtWin>
#include <QTextStream>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QThread>
#include <QProgressBar>
#include <QGraphicsScene>
#include "l2window.h"
#include "dongle.h"
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
    QPushButton *keysettings[KEYNUM];
    QThread* dongle_thread;
    Dongle* dongle;
    QProgressBar *pb[BARNUM];
    int ellipsed_time;
    static char* StyleSheet[BARNUM+1];

    void enumerateL2();
    bool isValidIndex(int index);

public slots:
    void showDongleStatus(int state, int updatetime); /* */
    void cbClicked(bool checked);
    void cbKeyEnableClicked(bool checked);
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
signals:
    void setDongleState(int stt);
};

#endif // MAINWINDOW_H
