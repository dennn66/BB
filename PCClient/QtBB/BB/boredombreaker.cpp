//789
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

char* BoredomBreaker::StyleSheetCheckBox[5] = {
    "QCheckBox::indicator  {background: #805300 ;}",  //YELLOW
    "QCheckBox::indicator  {background: #881D18 ;}",  //RED
    "QCheckBox::indicator  {background: #03327E ;}",  //BLUE
    "QCheckBox::indicator  {background: #318A10 ;}",  //GREEN
    "QCheckBox::indicator  {background: #7F7F7F ;}"   //GREY
};

char* BoredomBreaker::StyleSheetLabel[2] = {
    "QLabel  {background: #805300 ;}",  //YELLOW
    "QLabel  {background: #7f7f7f ;}"  //GREY
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
    bModifier = false;

    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);

    default_file_name = sett.value("MAIN/DefaultProject", "default.bbproj").toString();

    bEnableSound = sett.value("MAIN/bEnableSound", 1).toBool();
    bEnableModifier = sett.value("MAIN/EnableModifier", 1).toBool();
    QVariant v_cond = sett.value("MAIN/ModifierCode", VK_LSHIFT);
    QString s_cond = v_cond.toString();
    if(strlen(s_cond.toStdString().c_str()) < 1){
        vkModifierCode = VK_LSHIFT;
    } else {
        vkModifierCode = s_cond.toInt();
    }
    if(vkModifierCode <0 || vkModifierCode > 0xFF)  vkModifierCode = VK_LSHIFT;
    v_cond = sett.value("MAIN/ActivationKeyCode", VK_OEM_3);
    s_cond = v_cond.toString();
    if(strlen(s_cond.toStdString().c_str()) < 1){
        vkActivationKeyCode = VK_OEM_3;
    } else {
        vkActivationKeyCode = s_cond.toInt();
    }
    if(vkActivationKeyCode <0 || vkActivationKeyCode > 0xFF)  vkActivationKeyCode = VK_OEM_3;



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

    QGridLayout *layout_2 = new QGridLayout;
    QString key_label = "B";
    QTextStream key_label_stream(&key_label);

    group_enable[0] = 1;
    for(int i = 1; i<GROUPSNUM; i++){
        group_enable[i] = 0;
    }

    for(int i=0;i<GROUPSNUM;i++)
    {
        int j=0;
        key_label = "B";
        key_label_stream <<  i+1;
        QGridLayout *sell  = new QGridLayout;
        keyenable2[i] = new QCheckBox(key_label.toStdString().c_str());
        sell->addWidget(keyenable2[i],0, 0);
        keyenable2[i]->setChecked (group_enable[i]);
        layout_2->addLayout(sell,i, j);
    }
    ui->chkbox_widget_2->setLayout(layout_2);

    connect(ui->cbDongle, SIGNAL(clicked(bool)), SLOT(cbDongleClicked(bool)));
    connect(ui->cbCtrl, SIGNAL(clicked(bool)), SLOT(cbCtrlShiftClicked(bool)));
    connect(ui->cbShift, SIGNAL(clicked(bool)), SLOT(cbCtrlShiftClicked(bool)));
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
    for(int i = 0; i< GROUPSNUM; i++){
        connect(keyenable2[i], SIGNAL(clicked(bool)), SLOT(cbKeyEnableBxClicked(bool)));
    }
    dongle_thread = new QThread;
    dongle = new Dongle();


    dongle->moveToThread(dongle_thread);
    dongle_thread->start();


    l2_parser_thread = new QThread;
    l2_parser = new L2parser();
    l2_parser->moveToThread(l2_parser_thread);
    l2_parser_thread->start();


    kb = SystemKeyboardReadWrite::instance();
    kb->setConnected(true);


    //QObject::connect(dongle, SIGNAL(error(QString)), thread, SLOT(errorString(QString)));

    connect(dongle_thread, SIGNAL(started()), dongle, SLOT(process()));
    connect(dongle, SIGNAL(finished()), dongle_thread, SLOT(quit()));
    connect(dongle, SIGNAL(finished()), dongle, SLOT(deleteLater()));
    connect(dongle_thread, SIGNAL(finished()), dongle_thread, SLOT(deleteLater()));

    connect(l2_parser_thread, SIGNAL(started()), l2_parser, SLOT(process()));
    connect(l2_parser, SIGNAL(finished()), l2_parser_thread, SLOT(quit()));
    connect(l2_parser, SIGNAL(finished()), l2_parser, SLOT(deleteLater()));
    connect(l2_parser_thread, SIGNAL(finished()), l2_parser_thread, SLOT(deleteLater()));


    connect(kb, SIGNAL(keyPressed(DWORD )), SLOT(keyGlobalPressed(DWORD)));
    connect(kb, SIGNAL(keyReleased(DWORD)), SLOT(keyGlobalReleased(DWORD)));

    connect(dongle, SIGNAL(showStatus(int, int, int)), this, SLOT(showDongleStatus(int, int, int)));
    enumerateL2();

    qDebug("BoredomBreaker start play");

      if(bEnableSound) QSound::play("sounds/on.wav");

    qDebug("BoredomBreaker stop play");

}

BoredomBreaker::~BoredomBreaker()
{
    qDebug("BoredomBreaker::~BoredomBreaker");
    delete ui;
}

void BoredomBreaker::keyGlobalPressed(DWORD vkCode)
{
    if(vkCode == vkModifierCode)  bModifier = true;

}

//bEnableModifier = true;
//vkModifierCode = VK_LSHIFT;
//vkActivationKeyCode = VK_OEM_3;

void BoredomBreaker::keyGlobalReleased(DWORD vkCode)
{

    if(bModifier  == false && bEnableModifier == true) return;
    if(vkCode == vkModifierCode) bModifier = false;
    if(vkCode == vkActivationKeyCode)  dongle->doSetState(!ui->cbDongle->isChecked());
}


void BoredomBreaker::cbDongleClicked(bool checked){
    qDebug("Checkbox: %d", checked);
    dongle->doSetState(checked);

}

void BoredomBreaker::cbCtrlShiftClicked(bool checked){
    qDebug("Checkbox: %d", checked);

    dongle->sendCMD_SET_MODIFIER(ui->cbCtrl->isChecked(), ui->cbShift->isChecked());
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

void BoredomBreaker::cbKeyEnableBxClicked(bool checked){
    qDebug("BoredomBreaker::cbKeyEnableBxClicked(bool checked): %d", checked);
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

void BoredomBreaker::toggleGroup(int group){
    qDebug("BoredomBreaker::enableGroup(int group): %d", group);
    if(group_enable[group]){
        enableGroup(group, false);
    } else {
        enableGroup(group, true);
    }
}

void BoredomBreaker::enableGroup(int group, bool state){
    qDebug("BoredomBreaker::enableGroup(int group): %d", group);
    group_enable[group] = state;
    keyenable2[group]->setChecked (group_enable[group]);
    dongle->setGroupsState(getGroupsBinaryState());

}


unsigned char BoredomBreaker::getGroupsBinaryState(){
    qDebug("KeyCondition::getGroupState()");
    unsigned char state = 0;
    for(int i = 0; i<CONDBNUM; i++){
        state |= group_enable[i] << (i+2);
    }
    state &= 0b00111100;
    return(state);
}
bool BoredomBreaker::getGroupBoolState(int group_num, unsigned char bin_state){
    qDebug("KeyCondition::getGroupState()");
    bin_state &= (1 << (group_num+2));
    if((bin_state & (1 << (group_num+2))) == 0){
        return(false);
    } else {
        return(true);
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
    l2_parser->setActiveL2W(l2list[index]);
}

void BoredomBreaker::showDongleStatus(int state, int g_state, int updatetime)
{
    //qDebug("BoredomBreaker::showDongleStatus");
    if(g_state == GROUPS_DISCONNECTED) {
        for(int i=0;i<GROUPSNUM;i++)
        {
            keyenable2[i]->setChecked(group_enable[i]);
            keyenable2[i]->setEnabled (false);
        }
    } else {
        for(int i=0;i<GROUPSNUM;i++)
        {
            keyenable2[i]->setChecked(getGroupBoolState(i, g_state));
            keyenable2[i]->setEnabled (true);
        }
    }

    switch(state){
    case STATE_OFF:
        if( ui->cbDongle->isChecked() == true){
            if(bEnableSound) QSound::play("sounds/off.wav");
           //  QSound::play("sounds/on.wav");
        }
        ui->cbDongle->setChecked(false);
        ui->cbDongle->setEnabled(true);
//        ui->cbDongle->setStyleSheet(StyleSheetCheckBox[1]); //RED
        break;
    case STATE_ON:
        if( ui->cbDongle->isChecked() == false){
            //QSound::play("sounds/off.wav");
             if(bEnableSound) QSound::play("sounds/on.wav");
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

    ellipsed_time=((ellipsed_time*5)+updatetime)/6;
    QString label;
    QTextStream(&label) << ellipsed_time << " ms";
    ui->lb_ellipsed_time->setText(label);


    int index = ui->cmbWinList->currentIndex();
    if(!isValidIndex(index))return;
    label = "";

    switch(l2list[index]->targettype){
        case  TARGETMEORPET:
            label = "ME OR PET";
            break;
        case  TARGETCHAR:
            label = "CHAR OR NPC";
            break;
        case  TARGETMOB:
            label = "MOB";
            break;
        case  NOTARGET:
            label = "NO TARGET";
            break;
        default:
            label = "UNKNOWN";
            break;
    }

    ui->lbTargetType->setText(label);

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

    QColor color = *l2list[index]->getStarColor();
    QPalette sample_palette;
    sample_palette.setColor(QPalette::Window, color.rgb());
    if(l2list[index]->getStarState()) {
        sample_palette.setColor(QPalette::WindowText, Qt::green);
    } else {
        sample_palette.setColor(QPalette::WindowText, color.rgb());
    }

    ui->lbStar->setAutoFillBackground(true);
    ui->lbStar->setPalette(sample_palette);

//    ui->lbStar->set   ///setPixmap(*l2list[index]->getIcon()->pixmap(ui->lbStar));
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
    qDebug("BoredomBreaker::addL2Window(HWND hwnd): %d", (int) hwnd);
    if(!l2list.isEmpty())
    {
        for(int index = 0; index < l2list.size(); index++){
            if(l2list[index]->getHWND() == hwnd) return;
        }
    }
    L2Window *l2w = new L2Window(hwnd);
    l2list.append(l2w);
    ui->cmbWinList->addItem(l2w->getTitle());

    l2w->LoadProject(default_file_name);

    cmbWinListActivated(ui->cmbWinList->currentIndex());
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)
{
        qDebug("BOOL CALLBACK EnumWindowsProc(HWND hwnd,LPARAM lParam)");
        char szTextWin[255];DWORD dwPID = (DWORD) NULL;
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
