#ifndef CLICKER_H
#define CLICKER_H

#include <QWidget>
#include <QtGui>
#include <QVBoxLayout>
#include "keysettingsdialog.h"
#include "dongle.h"

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



public slots:
    void cbDongleClicked(bool checked);
    void cbCtrlShiftClicked(bool checked);
    void cbKeyEnableBxClicked(bool checked);
    void showDongleStatus(unsigned char d_stt, int updatetime); /* */


signals:
    void setDongleGroupState(int i, bool state);
    void doSetState(bool stt);
    void doSetModifier(bool bCtrl, bool bShift);
};

#endif // CLICKER_H
