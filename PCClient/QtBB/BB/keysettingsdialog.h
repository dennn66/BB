#ifndef KEYSETTINGSDIALOG_H
#define KEYSETTINGSDIALOG_H

#include <QDialog>
#include <QTextStream>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>


#include "keycondition.h"


namespace Ui {
class KeySettingsDialog;
}

class KeySettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeySettingsDialog(KeyCondition* con, QImage* tool, QWidget *parent = 0);
    ~KeySettingsDialog();

private:
    Ui::KeySettingsDialog *ui;
    QLineEdit* controlf[CONDFNUM];
    QLineEdit* controli[CONDINUM];
    QCheckBox* controlb[CONDBNUM];
    KeyCondition* condition;
    //QCheckBox *keyenable2[GROUPSNUM];
    //QCheckBox *targettype[TARGETTYPENUM];

public slots:
    void textFChanged(QString text);
    void textIChanged(QString text);
    void cbKeyMnemonicActivated(int);
    void cbCtrlClicked(bool checked);
    void cbShiftClicked(bool checked);
    void cbKeyEnableBxClicked(bool checked);
};

#endif // KEYSETTINGSDIALOG_H
