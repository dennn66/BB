#include "keysettingsdialog.h"
#include "ui_keysettingsdialog.h"
#include "dongle.h"

KeySettingsDialog::KeySettingsDialog(KeyCondition* con, QImage* tool, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::KeySettingsDialog)
{
    ui->setupUi(this);
    QDoubleValidator *vd = new QDoubleValidator();
    QIntValidator * vi = new QIntValidator();
    condition = con;
    QString value;
    QTextStream out(&value);

    controlf[idCoolDown      ] = ui->leCoolDown;
    controlf[idPause         ] = ui->lePause;
    controlf[idCondition     ] = ui->leCondition;

    controli[idMinCP         ] = ui->leCPMin;
    controli[idMinHP         ] = ui->leHPMin;
    controli[idMinMP         ] = ui->leMPMin;
    controli[idMinVP         ] = ui->leVPMin;
    controli[idMinMobHP      ] = ui->leMobHPMin;
    controli[idMinMobMP      ] = ui->leMobMPMin;
    controli[idMaxCP         ] = ui->leCPMax;
    controli[idMaxHP         ] = ui->leHPMax;
    controli[idMaxMP         ] = ui->leMPMax;
    controli[idMaxVP         ] = ui->leVPMax;
    controli[idMaxMobHP      ] = ui->leMobHPMax;
    controli[idMaxMobMP      ] = ui->leMobMPMax;

    for(int i = 0; i<KEY_DB_SIZE; i++){
        ui->cbKeyMnemonic->addItem(Dongle::KeyMnemonicDB[i]);
        if(strcmp(Dongle::KeyMnemonicDB[i], condition->KeyString.toStdString().c_str()) == 0){
            ui->cbKeyMnemonic->setCurrentIndex(i);
        }
    }
    connect(ui->cbKeyMnemonic, SIGNAL(activated(int)), SLOT(cbKeyMnemonicActivated(int)));
    connect(ui->cbCtrl, SIGNAL(clicked(bool)), SLOT(cbCtrlClicked(bool)));
    connect(ui->cbShift, SIGNAL(clicked(bool)), SLOT(cbShiftClicked(bool)));
    if(!tool->isNull())    ui->tool_label->setPixmap(QPixmap::fromImage(*tool));

    //ui->cbPermanent->setChecked(condition->permanent);
    ui->cbCtrl->setChecked(condition->ctrl);
    ui->cbShift->setChecked(condition->shift);

    for(int i = idCoolDown; i<CONDFNUM; i++){
        value = "";
        controlf[i]->setValidator(vd);
        connect(controlf[i], SIGNAL(textChanged(QString)), SLOT(textFChanged(QString)));
        out << condition->conditionf[i];
        controlf[i]->setText(value);
    }



    ui->leMPMin->setValidator(vi);
    for(int i = idMinCP; i<CONDINUM; i++){
        value = "";
        controli[i]->setValidator(vi);
        connect(controli[i], SIGNAL(textChanged(QString)), SLOT(textIChanged(QString)));
        if( condition->conditioni[i]<=100){
            out << condition->conditioni[i];
        }
        controli[i]->setText(value);
    }

    QGridLayout *layout_2 = new QGridLayout;
    QString key_label = "B";
    QTextStream key_label_stream(&key_label);

    for(int i=0;i<GROUPSNUM;i++)
    {
        int j=0;
        key_label = "B";
        key_label_stream <<  i+1;
        QGridLayout *sell  = new QGridLayout;
        controlb[idGroupB1+i] = new QCheckBox(key_label.toStdString().c_str());
        sell->addWidget(controlb[idGroupB1+i],0, 0);
        controlb[idGroupB1+i]->setChecked (condition->getGroupState(i));
        layout_2->addLayout(sell,i, j);
    }
    ui->chkbox_widget_2->setLayout(layout_2);
    controlb[idCheckStar] = ui->cbCheckStar;
    controlb[idCheckStar]->setChecked (condition->conditionb[idCheckStar]);
    controlb[idCheckSkillTimeout] = ui->cbCheckSkill;
    controlb[idCheckSkillTimeout]->setChecked (condition->conditionb[idCheckSkillTimeout]);
    controlb[idStarState] = ui->cbStarState;
    controlb[idStarState]->setChecked (condition->conditionb[idStarState]);



    QGridLayout *layout_3 = new QGridLayout;
    for(int i=0;i<2;i++)
    {
        for(int j=0;j<2;j++)
        {
        //int j=0;
            key_label = condition->conditionb_name[idTargetMeOrPet+i+j*2];
            QGridLayout *sell  = new QGridLayout;
            controlb[idTargetMeOrPet+i+j*2] = new QCheckBox(key_label.toStdString().c_str());
            sell->addWidget(controlb[idTargetMeOrPet+i+j*2],0, 0);
            controlb[idTargetMeOrPet+i+j*2]->setChecked (condition->conditionb[idTargetMeOrPet+i+j*2]);
            layout_3->addLayout(sell,i, j);
        }
    }
    ui->gbTargetType->setLayout(layout_3);

    for(int i=0;i<CONDBNUM;i++){
        connect(controlb[i], SIGNAL(clicked(bool)), SLOT(cbKeyEnableBxClicked(bool)));
    }


}

KeySettingsDialog::~KeySettingsDialog()
{
    delete ui;
}

void KeySettingsDialog::cbKeyEnableBxClicked(bool checked){
    qDebug("KeySettingsDialog::cbKeyEnableBxClicked(bool checked): %d", checked);
    QCheckBox* cb = dynamic_cast<QCheckBox*>(QObject::sender());
    if( cb != NULL )
    {
        int i = 0;
        while( (i < CONDBNUM) && controlb[i] != cb){i++;}
        if(i<CONDBNUM){
            condition->conditionb[i] = checked;
        }
    }
}

void KeySettingsDialog::textFChanged(QString text)
{
    QLineEdit* le = dynamic_cast<QLineEdit*>(QObject::sender());
    int i = idCoolDown;

    while( (i< CONDFNUM) && controlf[i] != le){i++;}
    if(i<CONDFNUM){
        condition->conditionf[i] = atof(text.toStdString().c_str());
    }
}

void KeySettingsDialog::textIChanged(QString text)
{
    QLineEdit* le = dynamic_cast<QLineEdit*>(QObject::sender());
    int i = idMinCP;
    qDebug("%s",text.toStdString().c_str() );
    while( (i< CONDINUM) && controli[i] != le){i++;}
    if(i<CONDINUM){
        if(text == "") {
            condition->conditioni[i] = 0xFF;
        } else {
            condition->conditioni[i] = atoi(text.toStdString().c_str());
            if( condition->conditioni[i]>100){
                condition->conditioni[i] = 0xFF;
            }
        }
    }
}


void KeySettingsDialog::cbCtrlClicked(bool checked){
    qDebug("Checkbox: %d", checked);
    condition->ctrl = checked;
    //dongle->doSetState(checked);

}

void KeySettingsDialog::cbShiftClicked(bool checked){
    qDebug("Checkbox: %d", checked);
    condition->shift = checked;
    //dongle->doSetState(checked);
}

void KeySettingsDialog::cbKeyMnemonicActivated(int index)
{
    condition->KeyString = Dongle::KeyMnemonicDB[index];
}
