#include "clicker.h"
#include "ui_clicker.h"

const char* Clicker::StyleSheetCheckBox[5] = {
    "QCheckBox::indicator  {background: #805300 ;}",  //YELLOW
    "QCheckBox { color : white; }; QCheckBox::indicator  {background: #881D18 ;}",  //RED
    "QCheckBox::indicator  {background: #03327E ;}",  //BLUE
    "QCheckBox { color : white; }; QCheckBox::indicator  {background: #318A10 ;}",  //GREEN
    "QCheckBox::indicator  {background: #7F7F7F ;}"   //GREY
};

Clicker::Clicker(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clicker)
{
    ui->setupUi(this);
    //setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setWindowFlags( Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::WindowTransparentForInput| Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground);

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, QColor("#0100FF00"));
    this->setPalette(pal);

    bFindBarsIsPressed = false;
    bSettingsIsPressed = false;
    green_frame = new QImage(QSize(51,23), QImage::Format_ARGB32);
    QPainter p;
    p.begin(green_frame);
    p.setPen(QPen(QColor("#4400FF00")));
    p.setBrush(QBrush(QColor("#4400FF00"), Qt::NoBrush));
    p.drawRect(QRect(0,0,50,22));
    p.setPen(QPen(QColor("#EE00FF00")));
    p.setBrush(QBrush(QColor("#8800FF00"), Qt::NoBrush));
    p.drawRect(QRect(1,1,48,20));
    p.setPen(QPen(QColor("#4400FF00")));
    p.setBrush(QBrush(QColor("#0100FF00"), Qt::SolidPattern));
    p.drawRect(QRect(2,2,46,18));
    p.end();
    red_frame = new QImage(QSize(51,23), QImage::Format_ARGB32);
    p.begin(red_frame);
    p.setPen(QPen(QColor("#44FF0000")));
    p.setBrush(QBrush(QColor("#44FF0000"), Qt::NoBrush));
    p.drawRect(QRect(0,0,50,22));
    p.setPen(QPen(QColor("#EEFF0000")));
    p.setBrush(QBrush(QColor("#88FF0000"), Qt::NoBrush));
    p.drawRect(QRect(1,1,48,20));
    p.setPen(QPen(QColor("#44FF0000")));
    p.setBrush(QBrush(QColor("#01FF0000"), Qt::SolidPattern));
    p.drawRect(QRect(2,2,46,18));
    p.end();

    QImage* bk_frame = new QImage(QSize(ui->lbBackground->width(),ui->lbBackground->height()), QImage::Format_ARGB32);
    p.begin(bk_frame);
    p.setPen(QPen(QColor("#20888888")));
    p.setBrush(QBrush(QColor("#20888888"), Qt::SolidPattern));
    p.drawRect(QRect(0,0,bk_frame->width()-1,bk_frame->height()-1));
    p.end();
    ui->lbBackground->setPixmap(QPixmap::fromImage(*bk_frame));



    pressed_btn = new QImage(QSize(51,23), QImage::Format_ARGB32);
    *pressed_btn = *green_frame;
    released_btn = new QImage(QSize(51,23), QImage::Format_ARGB32);
    *released_btn = *green_frame;

    //setWindowFlags(windowFlags()|Qt::WindowStaysOnTopHint);

    //ui->lcd_ellipsed_time->setAutoFillBackground(true);
    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);

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

void Clicker::drawStatusBtn(QImage* imgStatus, bool pressed, bool mainstatus, bool mobstatus, bool toolbarstatus){

    *imgStatus = *green_frame;
    QPainter p;
    p.begin(imgStatus);
    if(pressed){
        p.setPen(QPen(QColor("#4400FF00")));
        p.setBrush(QBrush(QColor("#4400FF00"), Qt::SolidPattern));
        p.drawRect(QRect(3,3,44,16));
    }
    if(mainstatus){
        p.setPen(QPen(QColor("#8800FF00")));
        p.setBrush(QBrush(QColor("#8800FF00"), Qt::SolidPattern));
    } else {
        p.setPen(QPen(QColor("#88FF0000")));
        p.setBrush(QBrush(QColor("#88FF0000"), Qt::SolidPattern));
    }
    p.drawRect(QRect(3,3,10,5));
    if(mobstatus){
        p.setPen(QPen(QColor("#8800FF00")));
        p.setBrush(QBrush(QColor("#8800FF00"), Qt::SolidPattern));
    } else {
        p.setPen(QPen(QColor("#88FF0000")));
        p.setBrush(QBrush(QColor("#88FF0000"), Qt::SolidPattern));
    }
    p.drawRect(QRect(18,3,10,4));
    if(toolbarstatus){
        p.setPen(QPen(QColor("#8800FF00")));
        p.setBrush(QBrush(QColor("#8800FF00"), Qt::SolidPattern));
    } else {
        p.setPen(QPen(QColor("#88FF0000")));
        p.setBrush(QBrush(QColor("#88FF0000"), Qt::SolidPattern));
    }
    p.drawRect(QRect(22,12,20,5));
/*
    p.setPen(QPen(QColor("#4400FF00")));
    p.setBrush(QBrush(QColor("#4400FF00"), Qt::NoBrush));
    p.drawRect(QRect(0,0,50,22));
    p.setPen(QPen(QColor("#EE00FF00")));
    p.setBrush(QBrush(QColor("#8800FF00"), Qt::NoBrush));
    p.drawRect(QRect(1,1,48,20));
    p.setPen(QPen(QColor("#4400FF00")));
    p.setBrush(QBrush(QColor("#4400FF00"), Qt::NoBrush));
    p.drawRect(QRect(2,2,46,18));
    p.end();
 */
}

void Clicker::showParserStatus(int updatetime, bool mainstatus, bool mobstatus, bool toolbarstatus){
    qDebug("Clicker::showParserStatus(int updatetime) %d", updatetime);
    static int ellipsed_time=0;
    ellipsed_time=((ellipsed_time*5)+updatetime)/6;

    QString label;
    QTextStream(&label) << ellipsed_time;
    ui->lcd_ellipsed_time->setDigitCount(5);
    if(ellipsed_time < 170){
        ui->lcd_ellipsed_time->setPalette(Qt::green);
    } else if(ellipsed_time < 200){
        ui->lcd_ellipsed_time->setPalette(Qt::yellow);
    } else {
        ui->lcd_ellipsed_time->setPalette(Qt::red);
    }
    ui->lcd_ellipsed_time->display(label);

    if(ui->cbDongle->isChecked()){
        ui->lbOnOff->setPixmap(QPixmap::fromImage(*green_frame));
    } else {
        ui->lbOnOff->setPixmap(QPixmap::fromImage(*red_frame));
    }
    if(ui->cbB1->isChecked()){
        ui->lbB1->setPixmap(QPixmap::fromImage(*green_frame));
    } else {
        ui->lbB1->setPixmap(QPixmap::fromImage(*red_frame));
    }
    if(ui->cbB2->isChecked()){
        ui->lbB2->setPixmap(QPixmap::fromImage(*green_frame));
    } else {
        ui->lbB2->setPixmap(QPixmap::fromImage(*red_frame));
    }
    if(ui->cbB3->isChecked()){
        ui->lbB3->setPixmap(QPixmap::fromImage(*green_frame));
    } else {
        ui->lbB3->setPixmap(QPixmap::fromImage(*red_frame));
    }
    if(ui->cbB4->isChecked()){
        ui->lbB4->setPixmap(QPixmap::fromImage(*green_frame));
    } else {
        ui->lbB4->setPixmap(QPixmap::fromImage(*red_frame));
    }
    if(ui->cbShift->isChecked()){
        ui->lbShift->setPixmap(QPixmap::fromImage(*green_frame));
    } else {
        ui->lbShift->setPixmap(QPixmap::fromImage(*red_frame));
    }
    if(ui->cbCtrl->isChecked()){
        ui->lbCtrl->setPixmap(QPixmap::fromImage(*green_frame));
    } else {
        ui->lbCtrl->setPixmap(QPixmap::fromImage(*red_frame));
    }
    drawStatusBtn(pressed_btn, true,  mainstatus,  mobstatus,  toolbarstatus);
    drawStatusBtn(released_btn, false,  mainstatus,  mobstatus,  toolbarstatus);
    if(bFindBarsIsPressed){
        if(!pressed_btn->isNull()) ui->lbStatus->setPixmap(QPixmap::fromImage(*pressed_btn));
    } else {
        if(!released_btn->isNull()) ui->lbStatus->setPixmap(QPixmap::fromImage(*released_btn));
    }
    //ui->pbStatus->setPixmap(QPixmap::fromImage(iStatus));


}

// Broadcasts a key has been pressed
void Clicker::keyLPressed(int x, int y){
    qDebug("Clicker::keyLPressed()");

    QPoint wdg = this->pos();
    if(x < (wdg.x())) return;
    if(x > (wdg.x()+this->width())) return;
    if(y < (wdg.y())) return;
    if(y > (wdg.y()+this->height())) return;

    if(isUnderWidget(ui->lbOnOff, x, y)) {
        emit doSetState(!ui->cbDongle->isChecked());
    } else if(isUnderWidget(ui->lbCtrl, x, y)) {
        emit doSetModifier(!ui->cbCtrl->isChecked(), ui->cbShift->isChecked());
    } else if(isUnderWidget(ui->lbShift, x, y)) {
        emit doSetModifier(ui->cbCtrl->isChecked(), !ui->cbShift->isChecked());
    } else if(isUnderWidget(ui->lbStatus, x, y)) {
        bFindBarsIsPressed = true;
        if(!pressed_btn->isNull())ui->lbStatus->setPixmap(QPixmap::fromImage(*pressed_btn));
    } else if(isUnderWidget(ui->lcd_ellipsed_time, x, y)) {
        bSettingsIsPressed = true;
        QPalette palette;
        palette.setBrush(QPalette::Background,QBrush(QColor("#88FF0000"), Qt::SolidPattern));
        ui->lcd_ellipsed_time->setPalette(palette);
    } else {
        QLabel* lbBx[4];
        lbBx[0] = ui->lbB1;
        lbBx[1] = ui->lbB2;
        lbBx[2] = ui->lbB3;
        lbBx[3] = ui->lbB4;

        int i = 0;
        while( (i < GROUPSNUM) && (!isUnderWidget(lbBx[i], x, y))){i++;}
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

    if(isUnderWidget(ui->lbStatus, x, y) && bFindBarsIsPressed) {
           // Reset window
        emit pbFindBarsClicked();
    }
    if(bFindBarsIsPressed){
        bFindBarsIsPressed = false;
        if(!released_btn->isNull())ui->lbStatus->setPixmap(QPixmap::fromImage(*released_btn));
    }
    //ui->cbDongle->setStyleSheet(StyleSheetCheckBox[2]); //BLUE


    if(isUnderWidget(ui->lcd_ellipsed_time, x, y)) {
        emit pbSettingsClicked();
    }
    if(bSettingsIsPressed){
            bSettingsIsPressed = false;
            QPalette palette;
            palette.setBrush(QPalette::Background,QBrush(QColor("#880000FF"), Qt::SolidPattern));
            ui->lcd_ellipsed_time->setPalette(palette);
    }

}


void Clicker::isL2Active(bool isActive, int right, int top)
{
    qDebug("Clicker::isActiveWindow(bool isActive, int right, int top): %d", isActive);

    QPoint topright;
    topright.setX(right-this->width()-right_offset);
    topright.setY(top+top_offset);
    this->move(topright);

    this->setVisible(this->underMouse() || isActive || (((HWND)(this->winId())) == ::GetForegroundWindow()));
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

    if((d_stt & (1<<DEVICE_STATUS)) == 0){
        ui->cbDongle->setChecked(false);
        ui->cbDongle->setEnabled(true);
   //     ui->cbDongle->setStyleSheet(StyleSheetCheckBox[1]); //RED
    } else if((d_stt & (1<<DEVICE_MODE)) == 0){
        ui->cbDongle->setChecked(true);
        ui->cbDongle->setEnabled(true);
   //     ui->cbDongle->setStyleSheet(StyleSheetCheckBox[3]); //GREEN
    } else {
        ui->cbDongle->setChecked(true);
        ui->cbDongle->setEnabled(true);
   //     ui->cbDongle->setStyleSheet(StyleSheetCheckBox[2]); //BLUE
    }

}


Clicker::~Clicker()
{
    qDebug("Clicker::~Clicker()");
    delete ui;
}
