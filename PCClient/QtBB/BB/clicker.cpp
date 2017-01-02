#include "clicker.h"
#include "ui_clicker.h"

const char* Clicker::StyleSheetCheckBox[5] = {
    "QCheckBox::indicator  {background: #805300 ;}",  //YELLOW
    "QCheckBox::indicator  {background: #881D18 ;}",  //RED
    "QCheckBox::indicator  {background: #03327E ;}",  //BLUE
    "QCheckBox::indicator  {background: #318A10 ;}",  //GREEN
    "QCheckBox::indicator  {background: #7F7F7F ;}"   //GREY
};

Clicker::Clicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clicker)
{
    ui->setupUi(this);
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput);
    setAttribute(Qt::WA_TranslucentBackground);


    //setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint);

    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);

    sett.value("MAIN/DefaultProject", "default.bbproj").toString();


    right_offset = sett.value("MAIN/RightOffset", 25).toInt();
    top_offset = sett.value("MAIN/TopOffset", 60).toInt();

    keyenable2[0] = ui->cbB1;
    keyenable2[1] = ui->cbB2;
    keyenable2[2] = ui->cbB3;
    keyenable2[3] = ui->cbB4;

    connect(ui->cbDongle, SIGNAL(clicked(bool)), SLOT(cbDongleClicked(bool)));
    connect(ui->cbCtrl, SIGNAL(clicked(bool)), SLOT(cbCtrlShiftClicked(bool)));
    connect(ui->cbShift, SIGNAL(clicked(bool)), SLOT(cbCtrlShiftClicked(bool)));


    for(int i = 0; i< GROUPSNUM; i++){
        connect(keyenable2[i], SIGNAL(clicked(bool)), SLOT(cbKeyEnableBxClicked(bool)));
    }

   // connect(dongle, SIGNAL(showStatus(int, int, int)), this, SLOT(showDongleStatus(int, int, int)));
    ms = SystemMouseHook::instance();
    ms->setConnected(true);
    connect(ms, SIGNAL(keyLPressed(int, int)), this, SLOT(keyLPressed(int, int)));
    connect(ms, SIGNAL(keyLReleased(int, int)), this, SLOT(keyLReleased(int, int)));


}


// Broadcasts a key has been pressed
void Clicker::keyLPressed(int x, int y){
    qDebug("Clicker::keyLPressed()");

    QPoint wdg = this->pos();
    if(x < (wdg.x())) return;
    if(x > (wdg.x()+40)) return;
    if(y < (wdg.y())) return;
    if(y > (wdg.y()+300)) return;

    if(isUnderWidget(ui->cbDongle, x, y)) {
        emit doSetState(!ui->cbDongle->isChecked());
    } else if(isUnderWidget(ui->cbCtrl, x, y)) {
        emit doSetModifier(!ui->cbCtrl->isChecked(), ui->cbShift->isChecked());
    } else if(isUnderWidget(ui->cbShift, x, y)) {
        emit doSetModifier(ui->cbCtrl->isChecked(), !ui->cbShift->isChecked());
    } else {
        int i = 0;
        while( (i < GROUPSNUM) && (!isUnderWidget(keyenable2[i], x, y))){i++;}
        if(i<GROUPSNUM){
            enableGroup(i, !keyenable2[i]->isChecked());
        }
    }
}

bool Clicker::isUnderWidget(QWidget* widget, int x, int y){
    QRect cb = widget->geometry();
    QPoint wdg = this->pos();
    qDebug("Clicker::keyLPressed() cb %d %d %d %d", cb.x(), cb.y(), cb.width(), cb.height());
    qDebug("Clicker::keyLPressed() wdg %d %d", wdg.x(), wdg.y());
    qDebug("Clicker::keyLPressed() clc %d %d", x, y);

    return (
                x > (wdg.x() + cb.x()) &&
                x < (wdg.x() + cb.x()+ cb.width()) &&
                y > (wdg.y() + cb.y()) &&
                y < (wdg.y() + cb.y()+ cb.height())

         );
}

// Broadcasts a key has been released
void Clicker::keyLReleased(int x, int y){
    qDebug("Clicker::keyLReleased()");
    Q_UNUSED(x);
    Q_UNUSED(y);

    //ui->cbDongle->setStyleSheet(StyleSheetCheckBox[2]); //BLUE

}


void Clicker::isL2Active(bool isActive, int right, int top)
{
    qDebug("Clicker::isActiveWindow(bool isActive, int right, int top): %d", isActive);

    QPoint topright;
    topright.setX(right-this->width()-right_offset);
    topright.setY(top+top_offset);
    this->move(topright);
    this->setVisible(isActive);
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
    emit doActivateL2();

}

void Clicker::cbDongleClicked(bool checked){
    qDebug("Clicker::cbDongleClicked(bool checked): %d", checked);
    //dongle->doSetState(checked);
    emit doSetState(checked);
    emit doActivateL2();
}

void Clicker::cbCtrlShiftClicked(bool checked){
    qDebug("Clicker::cbCtrlShiftClicked(bool checked: %d", checked);

    //dongle->sendCMD_SET_MODIFIER(ui->cbCtrl->isChecked(), ui->cbShift->isChecked());
    emit doSetModifier(ui->cbCtrl->isChecked(), ui->cbShift->isChecked());
    emit doActivateL2();
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

        ui->cbDongle->setChecked(false);
        ui->cbDongle->setEnabled(true);
        //ui->cbDongle->setStyleSheet(StyleSheetCheckBox[1]); //RED
        break;
    case STATE_ON:
        ui->cbDongle->setChecked(true);
        ui->cbDongle->setEnabled(true);
        //ui->cbDongle->setStyleSheet(StyleSheetCheckBox[2]); //BLUE
        break;
     default:
        ui->cbDongle->setChecked(false);
        ui->cbDongle->setEnabled(false);
        //ui->cbDongle->setStyleSheet(StyleSheetCheckBox[0]); // YELLOW
        break;
    }

}


Clicker::~Clicker()
{
    qDebug("Clicker::~Clicker()");
    delete ui;
}
