#include "keysettingsdialog.h"
#include "ui_keysettingsdialog.h"
#include "dongle.h"

KeySettingsDialog::KeySettingsDialog(KeyCondition* con, QWidget *parent) :
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

}

KeySettingsDialog::~KeySettingsDialog()
{
    delete ui;
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

void KeySettingsDialog::cbKeyMnemonicActivated(int index)
{
    condition->KeyString = Dongle::KeyMnemonicDB[index];
}
