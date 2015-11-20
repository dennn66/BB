#include "boredombreaker.h"
#include "ui_boredombreaker.h"

char* BoredomBreaker::StyleSheet[BARNUM+1] = {
    "QProgressBar::chunk {background: #805300 ;}",
    "QProgressBar::chunk {background: #881D18 ;}",
    "QProgressBar::chunk {background: #03327E ;}",
    "QProgressBar::chunk {background: #318A10 ;}",
   "QProgressBar::chunk {background: #881D18 ;}",
   "QProgressBar::chunk {background: #03327E ;}",
    "QProgressBar::chunk {background: #7F7F7F ;}"
};

BoredomBreaker::BoredomBreaker(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BoredomBreaker)
{
    ui->setupUi(this);
    ellipsed_time = 0;
    pb[idCP] = ui->barCP;
    pb[idHP] = ui->barHP;
    pb[idMP] = ui->barMP;
    pb[idVP] = ui->barVP;
    pb[idMobHP] = ui->barMobHP;
    pb[idMobMP] = ui->barMobMP;

    for(int j = idCP; j < BARNUM; j++ ){
        pb[j]->setMaximum(100);
        pb[j]->setMinimum(0);
        pb[j]->setStyleSheet(StyleSheet[BARNUM]);
        pb[j]->setValue(100);
        pb[j]->setVisible(true);
    }

    QGridLayout *layout = new QGridLayout;
    for(int i=0;i<4;i++)
    {
        for(int j=0;j<12;j++)
        {
            int index = (3-i)*12+j;
            QGridLayout *sell  = new QGridLayout;
            QGridLayout *halfsell  = new QGridLayout;
            keylabel[index] = new QLabel("");
            keyenable[index] = new QCheckBox("");
            keysettings[index] = new QPushButton("");
            halfsell->addWidget(keyenable[index],0, 0);
            halfsell->addWidget(keysettings[index],0, 1);
            keyenable[index]->setChecked (false);
            sell->addWidget(keylabel[index],0, 0);
            sell->addLayout(halfsell,1, 0);
            layout->addLayout(sell,i, j);
        }
    }
    ui->chkbox_widget->setLayout(layout);

    connect(ui->cbDongle, SIGNAL(clicked(bool)), SLOT(cbClicked(bool)));
    connect(ui->pbLoadProject, SIGNAL(clicked()), SLOT(pbLoadProjectClicked()));
    connect(ui->pbSaveProject, SIGNAL(clicked()), SLOT(pbSaveProjectClicked()));
    connect(ui->pbLoad, SIGNAL(clicked()), SLOT(pbLoadClicked()));
    connect(ui->pbSave, SIGNAL(clicked()), SLOT(pbSaveClicked()));
    connect(ui->pbAdd, SIGNAL(clicked()), SLOT(pbAddClicked()));
    connect(ui->pbDongle, SIGNAL(clicked()), SLOT(pbToDongleClicked()));
    connect(ui->pbFindBars, SIGNAL(clicked()), SLOT(pbFindBarsClicked()));
    connect(ui->pbEnumerate, SIGNAL(clicked()), SLOT(pbEnumerateClicked()));
    connect(ui->cmbWinList, SIGNAL(activated(int)), SLOT(cmbWinListActivated(int)));
    connect(ui->cmbCondSetList, SIGNAL(activated(int)), SLOT(cmbCondSetListActivated(int)));
    connect(ui->leNic, SIGNAL(textChanged(const QString &)), SLOT(cmbCondSetListTextChanged(const QString &)));

    for(int i = 0; i< 48; i++){
        connect(keyenable[i], SIGNAL(clicked(bool)), SLOT(cbKeyEnableClicked(bool)));
        connect(keysettings[i], SIGNAL(clicked()), SLOT(pbKeySettingsClicked()));
    }

    dongle_thread = new QThread;
    dongle = new Dongle();

    dongle->moveToThread(dongle_thread);
    dongle_thread->start();
    //QObject::connect(dongle, SIGNAL(error(QString)), thread, SLOT(errorString(QString)));

    connect(dongle_thread, SIGNAL(started()), dongle, SLOT(process()));
    connect(dongle, SIGNAL(finished()), dongle_thread, SLOT(quit()));
    connect(dongle, SIGNAL(finished()), dongle, SLOT(deleteLater()));
    connect(dongle_thread, SIGNAL(finished()), dongle_thread, SLOT(deleteLater()));


    connect(dongle, SIGNAL(showStatus(int, int)), this, SLOT(showDongleStatus(int, int)));
    enumerateL2();
}

BoredomBreaker::~BoredomBreaker()
{
    qDebug("BoredomBreaker::~BoredomBreaker");
    delete ui;
}

void BoredomBreaker::cbClicked(bool checked){
    qDebug("Checkbox: %d", checked);
    dongle->doSetState(checked);

}

void BoredomBreaker::cmbCondSetListTextChanged(const QString &text){
    qDebug("BoredomBreaker::cmbCondSetListTextChanged: %s", text.toStdString().c_str());
    int l2_index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(l2_index))return;
    int index = ui->cmbCondSetList->currentIndex();
    if(!l2list[l2_index]->isValidIndex(index)) return;

    l2list[l2_index]->getCurrentSettings()->nic = text;
    ui->cmbCondSetList->setItemText(index, l2list[l2_index]->getNic());
}

//cbKeyEnableClicked
void BoredomBreaker::cbKeyEnableClicked(bool checked){
    qDebug("BoredomBreaker::cbKeyEnableClicked(bool checked): %d", checked);
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    //QObject* obj = sender();
    QCheckBox* cb = dynamic_cast<QCheckBox*>(QObject::sender());
    if( cb != NULL )
    {
        int i = 0;
        while( (i< 48) && keyenable[i] != cb){i++;}
        if(i<48){
            l2list[index]->getCurrentSettings()->condition[i]->FSet = checked;
            dongle->doSendKeyToDongle(i);
            //cmbWinListActivated(index);
        }
    }
}

bool BoredomBreaker::isValidIndex(int index){
    if((index == -1)||(l2list.isEmpty())||(index >= l2list.size()))return false;
    return true;
}

void BoredomBreaker::pbKeySettingsClicked(){
    qDebug("BoredomBreaker::pbKeySettingsClicked()");
    QPushButton* pb = dynamic_cast<QPushButton*>(QObject::sender());
    if( pb != NULL )
    {
        int i = 0;
        while( (i< 48) && keysettings[i] != pb){i++;}

        if(i<48){
            int index = ui->cmbWinList->currentIndex();
            if(!isValidIndex(index))return;

            KeyCondition cond(".");
            cond = *(l2list[index]->getCurrentSettings()->condition[i]);

            KeySettingsDialog dlg(&cond);
            if(dlg.exec() == QDialog::Accepted){
               *(l2list[index]->getCurrentSettings()->condition[i]) = cond;
            }
            dongle->doSendKeyToDongle(i);
        }
    }
}

void BoredomBreaker::pbAddClicked(){
    qDebug("BoredomBreaker::pbAddClicked");
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;

    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.cfg");
    if(file_name.isEmpty() || file_name.isNull()) return;
    l2list[index]->AddConfig(file_name);
    pbToDongleClicked();
    ui->cmbCondSetList->addItem(l2list[index]->getCurrentSettings()->nic);
    ui->cmbCondSetList->setCurrentIndex(l2list[index]->activeCondSet);
    cmbCondSetListActivated(l2list[index]->activeCondSet);
}

void BoredomBreaker::pbLoadProjectClicked(){
    qDebug("BoredomBreaker::pbLoadProjectClicked");
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;

    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.bbproj");
    if(file_name.isEmpty() || file_name.isNull()) return;
    l2list[index]->LoadProject(file_name);
    cmbWinListActivated(index);
    ui->cmbCondSetList->setCurrentIndex(l2list[index]->activeCondSet);
    cmbCondSetListActivated(l2list[index]->activeCondSet);
    pbToDongleClicked();
}

void BoredomBreaker::pbSaveProjectClicked(){
    qDebug("BoredomBreaker::pbSaveProjectClicked");

    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    QString file_name = QFileDialog::getSaveFileName(this, QString::fromUtf8("Имя файла"), l2list[index]->project_file_name);
    if(file_name.isEmpty() || file_name.isNull()) return;
    qDebug("filename: %s", file_name.toStdString().c_str());
    l2list[index]->SaveProject(file_name);
    return;
}


void BoredomBreaker::pbLoadClicked(){
    qDebug("BoredomBreaker::pbLoadClicked");
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;

    QString file_name = QFileDialog::getOpenFileName(this, QString::fromUtf8("Имя файла"), "*.cfg");
    if(file_name.isEmpty() || file_name.isNull()) return;
    l2list[index]->LoadConfig(file_name);
    pbToDongleClicked();
    ui->cmbCondSetList->setCurrentIndex(l2list[index]->activeCondSet);
    cmbCondSetListActivated(l2list[index]->activeCondSet);
}

void BoredomBreaker::pbSaveClicked(){
    qDebug("BoredomBreaker::pbSaveClicked");

    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    QString file_name = QFileDialog::getSaveFileName(this, QString::fromUtf8("Имя файла"), l2list[index]->getCurrentSettings()->settings_file_name);
    if(file_name.isEmpty() || file_name.isNull()) return;
    qDebug("filename: %s", file_name.toStdString().c_str());
    l2list[index]->SaveConfig(file_name);
    return;

    // Тест вывода фона под кнопки
    QString imagePath = QFileDialog::getOpenFileName(
                    this,
                    tr("Open File"),
                    "",
                    tr("PNG (*.png);;JPEG (*.jpg *.jpeg)" )
                    );
    QImage* imageObject = new QImage();
    imageObject->load(imagePath);
    ui->key_label->setPixmap(QPixmap::fromImage(*imageObject));
    ui->key_label->show();

    return;

}

void BoredomBreaker::pbToDongleClicked(){
    qDebug("BoredomBreaker::pbToDongleClicked");
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    dongle->doSaveAllToDongle();
}

void BoredomBreaker::pbEnumerateClicked(){
    qDebug("BoredomBreaker::pbEnumerateClicked()");
    enumerateL2();
}

void BoredomBreaker::pbFindBarsClicked(){
    qDebug("BoredomBreaker::pbFindBarsClicked()");
    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    l2list[index]->findBars();
}

void BoredomBreaker::cmbCondSetListActivated(int index){

    qDebug("BoredomBreaker::cmbWinListActivated");
    int l2_index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(l2_index))return;

    if(!l2list[l2_index]->isValidIndex(index)) return;
    l2list[l2_index]->activateSettings(index);

    for(int i=0;i<48;i++)
    {
        keylabel[i]->setText(l2list[l2_index]->getConditionLabel(i));
        keyenable[i]->setToolTip(l2list[l2_index]->getConditionLabel(i));
        keylabel[i]->setToolTip(l2list[l2_index]->getConditionLabel(i));
        keysettings[i]->setToolTip(l2list[l2_index]->getConditionLabel(i));
        keyenable[i]->setChecked (l2list[l2_index]->getConditionState(i));

    }
    ui->lbConfFileName->setText(l2list[l2_index]->getCurrentSettings()->settings_file_name);
    ui->leNic->setText(l2list[l2_index]->getNic());

}

void BoredomBreaker::cmbWinListActivated(int index){

    qDebug("BoredomBreaker::cmbWinListActivated");
    if(!isValidIndex(index))return;

    ui->cmbWinList->setItemIcon(index, *l2list[index]->getIcon());
    ui->cmbWinList->setItemText(index, l2list[index]->getTitle());
    while(ui->cmbCondSetList->count() > 0) {
        ui->cmbCondSetList->removeItem(0);
    }

    if(!l2list[index]->cond_set_list.isEmpty()){
      for(int i=0; i < l2list[index]->cond_set_list.size(); i++) {
          ui->cmbCondSetList->addItem(l2list[index]->cond_set_list[i]->nic);
      }
    }

    cmbCondSetListActivated(l2list[index]->activeCondSet);
    dongle->setActiveL2W(l2list[index]);
}

void BoredomBreaker::showDongleStatus(int state, int updatetime)
{
    //qDebug("BoredomBreaker::showDongleStatus");
    switch(state){
    case STATE_DISCONNECTED:
        ui->cbDongle->setChecked(false);
        ui->cbDongle->setEnabled(false);
        break;
    case STATE_OFF:
        ui->cbDongle->setChecked(false);
        ui->cbDongle->setEnabled(true);
        break;
    case STATE_ON:
        ui->cbDongle->setChecked(true);
        ui->cbDongle->setEnabled(true);
        break;
     default:
        ui->cbDongle->setChecked(true);
        ui->cbDongle->setEnabled(false);
        break;
    }

    ellipsed_time=((ellipsed_time*5)+updatetime)/6;
    QString label;
    QTextStream(&label) << ellipsed_time << " ms";
    ui->lb_ellipsed_time->setText(label);

    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;

    for(int j = idCP; j < BARNUM; j++ ){
        int xp = l2list[index]->getXP(j);
        if(xp >=0 && xp <= 100){
            pb[j]->setValue(xp);
            pb[j]->setStyleSheet(StyleSheet[j]);
        } else {
            pb[j]->setValue(100);
            pb[j]->setStyleSheet(StyleSheet[BARNUM]);
        }
    }

    ui->cmbWinList->setItemIcon(index, *l2list[index]->getIcon());
    ui->cmbWinList->setItemText(index, l2list[index]->getTitle());

}


void BoredomBreaker::enumerateL2(){
    qDebug("BoredomBreaker::enumerateL2()");
    if((BOOL)EnumWindows(&EnumWindowsProc,reinterpret_cast<long int>(this)))
    {
            qDebug("EnumWindows - ok");
    }else{qDebug("EnumWindows - not ok");}

}

void BoredomBreaker::addL2Window(HWND hwnd){
    qDebug("BoredomBreaker::addL2Window(HWND hwnd): %d", hwnd);
    if(!l2list.isEmpty())
    {
        for(int index = 0; index < l2list.size(); index++){
            if(l2list[index]->getHWND() == hwnd) return;
        }
    }
    L2Window *l2w = new L2Window(hwnd);
    l2list.append(l2w);
    ui->cmbWinList->addItem(l2w->getTitle());
    cmbWinListActivated(ui->cmbWinList->currentIndex());
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
        qDebug("BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)");
        char szTextWin[255];DWORD dwPID = NULL;
        GetWindowTextA(hwnd,szTextWin,sizeof(szTextWin));
        if(strstr(szTextWin, "Lineage") != 0){
            qDebug(" - Txt: %s", szTextWin);
            qDebug(" - TID: %d", GetWindowThreadProcessId(hwnd,&dwPID));
            qDebug(" - PID: %d", dwPID);
            qDebug("  HWND: %d", hwnd);
             //Capture(hwnd);
            BoredomBreaker * bb = reinterpret_cast<BoredomBreaker*>(lParam);
            bb->addL2Window(hwnd);
        }
        //szTextWin[0] = '\0';
        return TRUE;
}