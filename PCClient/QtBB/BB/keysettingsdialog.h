#ifndef KEYSETTINGSDIALOG_H
#define KEYSETTINGSDIALOG_H

#include <QDialog>
#include <QTextStream>
#include <QLineEdit>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QListWidget>

#include "ui_keysettingsdialog.h"
#include "dongle.h"
#include "keycondition.h"
#include "l2window.h"

namespace Ui {
class KeySettingsDialog;
}




class KeySettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit KeySettingsDialog(KeyCondition* con, L2Window* l2w, int tool, QWidget *parent = 0);
    ~KeySettingsDialog();

private:

    QListWidget* lstAllSkills;
    Ui::KeySettingsDialog *ui;
    QLineEdit* controlf[CONDFNUM];
    QLineEdit* controli[CONDINUM];
    QCheckBox* controlb[CONDBNUM];
    L2Window* currentl2w;
    int toolNumber;
    QListWidgetItem *listNoUseSkill[idNoUseSkillNum];
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
    void itemChanged(QListWidgetItem* item);
};

#endif // KEYSETTINGSDIALOG_H
