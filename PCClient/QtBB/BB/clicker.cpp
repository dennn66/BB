#include "clicker.h"
#include "ui_clicker.h"


Clicker::Clicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clicker)
{
    ui->setupUi(this);
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint );
    setAttribute(Qt::WA_TranslucentBackground);
    //setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint);

    QGridLayout *layout_2 = new QGridLayout;
    QString key_label = "B";
    QTextStream key_label_stream(&key_label);

    //group_enable[0] = 1;
    //for(int i = 1; i<GROUPSNUM; i++){
    //    group_enable[i] = 0;
    //}

    for(int i=0;i<GROUPSNUM;i++)
    {
        int j=0;
        key_label = "B";
        key_label_stream <<  i+1;
        QGridLayout *sell  = new QGridLayout;
        keyenable2[i] = new QCheckBox(key_label.toStdString().c_str());
        sell->addWidget(keyenable2[i],0, 0);
        keyenable2[i]->setChecked (1);
        layout_2->addLayout(sell,i, j);
    }
    ui->chkbox_widget_2->setLayout(layout_2);

    connect(ui->cbDongle, SIGNAL(clicked(bool)), SLOT(cbDongleClicked(bool)));
    connect(ui->cbCtrl, SIGNAL(clicked(bool)), SLOT(cbCtrlShiftClicked(bool)));
    connect(ui->cbShift, SIGNAL(clicked(bool)), SLOT(cbCtrlShiftClicked(bool)));


    for(int i = 0; i< GROUPSNUM; i++){
        connect(keyenable2[i], SIGNAL(clicked(bool)), SLOT(cbKeyEnableBxClicked(bool)));
    }

   // connect(dongle, SIGNAL(showStatus(int, int, int)), this, SLOT(showDongleStatus(int, int, int)));


}


void Clicker::cbKeyEnableBxClicked(bool checked){
    qDebug("Clicker::cbKeyEnableBxClicked(bool checked): %d", checked);
    QCheckBox* cb = dynamic_cast<QCheckBox*>(QObject::sender());
    if( cb != NULL )
    {
        int i = 0;
        while( (i < GROUPSNUM) && keyenable2[i] != cb){i++;}
        if(i<GROUPSNUM){
            enableGroup(i, checked);
        }
    }
}

void Clicker::cbDongleClicked(bool checked){
    qDebug("Clicker::cbDongleClicked(bool checked): %d", checked);
    //dongle->doSetState(checked);
    emit doSetState(checked);

}

void Clicker::cbCtrlShiftClicked(bool checked){
    qDebug("Clicker::cbCtrlShiftClicked(bool checked: %d", checked);

    //dongle->sendCMD_SET_MODIFIER(ui->cbCtrl->isChecked(), ui->cbShift->isChecked());
    emit doSetModifier(ui->cbCtrl->isChecked(), ui->cbShift->isChecked());
}





void Clicker::enableGroup(int group, bool state){
    qDebug("Clicker::enableGroup(int group): %d", group);
    keyenable2[group]->setChecked (state);
    emit setDongleGroupState(group, state);
}


void Clicker::showDongleStatus(unsigned char d_stt, int updatetime)
{
    //qDebug("Clicker::showDongleStatus");
    //qDebug("BoredomBreaker::showDongleStatus");
    Q_UNUSED(updatetime);
    for(int i=0;i<GROUPSNUM;i++)
    {
        keyenable2[i]->setChecked((d_stt & (1<<(i+GROUP_0))) > 0);
        keyenable2[i]->setEnabled (true);
    }


    ui->cbCtrl->setChecked((d_stt & (1 << DEVICE_CTRL)) > 0);
    ui->cbShift->setChecked((d_stt & (1 << DEVICE_SHIFT)) > 0);

    bool state = (d_stt & (1<<DEVICE_STATUS)) > 0;
    switch(state){
    case STATE_OFF:
        if( ui->cbDongle->isChecked() == true){
            //if(bEnableSound) QSound::play("sounds/off.wav");
           //  QSound::play("sounds/on.wav");
        }
        ui->cbDongle->setChecked(false);
        ui->cbDongle->setEnabled(true);
//        ui->cbDongle->setStyleSheet(StyleSheetCheckBox[1]); //RED
        break;
    case STATE_ON:
        if( ui->cbDongle->isChecked() == false){
            //QSound::play("sounds/off.wav");
             //if(bEnableSound) QSound::play("sounds/on.wav");
        }
        ui->cbDongle->setChecked(true);
        ui->cbDongle->setEnabled(true);
//        ui->cbDongle->setStyleSheet(StyleSheetCheckBox[2]); //BLUE
        break;
     default:
        ui->cbDongle->setChecked(false);
        ui->cbDongle->setEnabled(false);
//        ui->cbDongle->setStyleSheet(StyleSheetCheckBox[0]); // YELLOW
        break;
    }

}


Clicker::~Clicker()
{
    qDebug("Clicker::~Clicker()");
    delete ui;
}
