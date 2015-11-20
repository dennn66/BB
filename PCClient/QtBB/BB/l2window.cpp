#include "l2window.h"
/*
char* L2Window::DefaultKeyDB[48] = {"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8","F9","F10","F11",                   "F12",
                                    "1",  "2",  "3",  "4",  "5",  "6",  "7",  "8", "9", "0",  "-",                     "=",
                                    "Q",  "W",  "E",  "R",  "T",  "Y",  "U",  "I", "O", "P",  "[",                     "]",
                                    "P1", "P2", "P3", "P4", "P5", "P6", "P7", "P8","P9","P0", "KEYPAD_DOT_AND_DELETE", "KEYPAD_SLASH"};

*/

L2Window::L2Window(HWND winhwnd)
{
    hwnd = winhwnd;
    status = L2_OFF;
    image_width = 0;
    image_height = 0;
    L2icon = NULL;

    bar[idCP].setColors(CP_COLOR, BK_CP_COLOR);
    bar[idHP].setColors(HP_COLOR, BK_HP_COLOR);
    bar[idMP].setColors(MP_COLOR, BK_MP_COLOR);
    bar[idVP].setColors(VP_COLOR, BK_VP_COLOR);
    bar[idMobHP].setColors(MobHP_COLOR, BK_MobHP_COLOR);
    bar[idMobMP].setColors(MobMP_COLOR, BK_MobMP_COLOR);

    KeyConditionsSet* conditionSet = new KeyConditionsSet();
    cond_set_list.append(conditionSet);
    project_file_name = "default.bbproj";
    activeCondSet = 0;


    for(int i = idCP; i < BARNUM; i++ ){
        bar[i].setbarID(i);
    }
}

QImage L2Window::capture(){
    QImage image;
//    qDebug("L2Window::capture");

    HWND handle = hwnd;
    HDC hdcSrc = GetWindowDC(handle);
    if(hdcSrc == NULL){
        qDebug("GetDC failed: %d", hwnd);
        return image;
    }

    RECT windowRect;
    GetWindowRect(handle, &windowRect);

    if(     (windowRect.left   !=    -32000) &&
            (windowRect.top    !=    -32000) &&
            (windowRect.left   <  4000) &&
            (windowRect.right  <  4000) &&
            (windowRect.top    <  4000) &&
            (windowRect.bottom <  4000) &&
            (windowRect.left   <  windowRect.right) &&
            (windowRect.top    <  windowRect.bottom)
        ){
        HDC hdcDest = CreateCompatibleDC(hdcSrc);
        int width = windowRect.right - windowRect.left;
        int height = windowRect.bottom - windowRect.top;
        HBITMAP hBitmap = CreateCompatibleBitmap(hdcSrc, width, height);
        HGDIOBJ hOld = SelectObject(hdcDest, hBitmap);
        BitBlt(hdcDest, 0, 0, width, height, hdcSrc, 0, 0, SRCCOPY);
        SelectObject(hdcDest, hOld);
        DeleteDC(hdcDest);
        ReleaseDC(handle, hdcSrc);
        QPixmap pixelMap = QtWin::fromHBITMAP(hBitmap);
        DeleteObject(hBitmap);
        //pixelMap.save("Lineage.png");
        image = pixelMap.toImage();
    }
    return image;
}

bool L2Window::findXP(int index, QImage image){
    qDebug("L2Window::findXP");
    RECT zone;
#define TOPMARGIN 0
    if(bar[index].getStatus() == BAR_OFF) {
        int i = idCP;

        switch (index) {
        case idCP:
        case idHP:
        case idMP:
        case idVP:
            zone.left = 0;
            zone.top = TOPMARGIN;
            zone.right = zone.left+400;
            zone.bottom = zone.top+220;
            break;
        case idMobMP:
        case idMobHP:
            while(i < idMobHP && bar[i].getStatus() != BAR_ON) i++;
            if(i < idMobHP){
                zone.left = bar[i].getEnd();
                zone.top = TOPMARGIN;
                zone.right = image.width()/2+400;
                zone.bottom = zone.top+190;
            } else {
                return(false);
            }
            break;
        default:
            return(false);
            break;
        }
        switch (index) {
        case idHP:
        case idMP:
        case idVP:
            if(bar[index-1].getStatus() == 1){
                zone.left = bar[index-1].getBegin();
                zone.right = bar[index-1].getEnd();
                zone.top = bar[index-1].getY()+9;
                zone.bottom = zone.top + 6;
            }
            break;
        case idMobMP:
            if(bar[index-1].getStatus() == 1){
                zone.left = bar[index-1].getBegin();
                zone.right = bar[index-1].getEnd();
                zone.top = bar[index-1].getY()+7;
                zone.bottom = zone.top + 10;
            }
            break;
        default:
            break;
        }
        bar[index].findXPBar(image,zone);
    }
    return bar[index].getStatus();
}


QIcon* L2Window::getIcon(){
//   qDebug("L2Window::getIcon");
    return L2icon;
}

HWND L2Window::getHWND(){
    qDebug("L2Window::getHWND");
    return hwnd;
}

bool L2Window::isValidIndex(int index){
    if((index == -1)||(cond_set_list.isEmpty())||(index >= cond_set_list.size()))return false;
    return true;
}

bool L2Window::activateSettings(int index){
    if(!isValidIndex(index))return false;
    activeCondSet = index;
    return true;
}

KeyConditionsSet* L2Window::getCurrentSettings(){
    if(!isValidIndex(activeCondSet))return NULL;
    return cond_set_list[activeCondSet];
}

int L2Window::LoadProject(QString file_name){
    qDebug("L2Window::LoadProject");
    QSettings sett(file_name, QSettings::IniFormat);

    project_file_name = file_name;
    qDebug("Proj File: %s", file_name.toStdString().c_str());
    int proj_size = sett.value("MAIN/count", 0).toInt();
    qDebug("count: %d", proj_size);
    if(proj_size < 1) return status;

    QString topic = "SETTINGS";

    for(int i = 0;i < proj_size;i++){
        QString var;
        QTextStream varstream(&var);
        var = topic;
        varstream  << "/settings_file_name" <<  i+1;
        qDebug("Topic: %s", var.toStdString().c_str());

        QString settings_file_name = sett.value(var.toStdString().c_str(), "default.cfg").toString();
        qDebug("Settings File: %s", settings_file_name.toStdString().c_str());

        if(i < cond_set_list.size()){
            activeCondSet = i;
            LoadConfig(settings_file_name);
            qDebug("Load: %d", i);

        } else {
            AddConfig(settings_file_name);
            qDebug("Add: %d", i);
        }
        activeCondSet = 0;
    }
    while(proj_size < cond_set_list.size()){
        KeyConditionsSet* tmp = cond_set_list.last();
        qDebug("Removed node %s", tmp->settings_file_name.toStdString().c_str());
        cond_set_list.removeLast();
        delete(tmp);
        qDebug("Removed node %s", tmp->settings_file_name.toStdString().c_str());
    }
    return status;
}

int L2Window::SaveProject(QString file_name){

    qDebug("L2Window::SaveProject");

    QSettings sett(file_name, QSettings::IniFormat);

    project_file_name = file_name;
    qDebug("File: %s", file_name.toStdString().c_str());
    sett.setValue("MAIN/count", cond_set_list.size());

    QString topic = "SETTINGS";

    for(int i = 0;i < cond_set_list.size();i++){
        QString var;
        QTextStream varstream(&var);
        var = topic;
        varstream  << "/settings_file_name" <<  i+1;
        sett.setValue(var.toStdString().c_str(), cond_set_list[i]->settings_file_name);
    }
    return status;
}

int L2Window::AddConfig(QString file_name){
    qDebug("L2Window::LoadConfig");
    KeyConditionsSet* conditionSet = new KeyConditionsSet();
    cond_set_list.append(conditionSet);
    activeCondSet = cond_set_list.size() - 1;
    getCurrentSettings()->LoadConfig(file_name);
    return status;
}

int L2Window::LoadConfig(QString file_name){
    qDebug("L2Window::LoadConfig");
    getCurrentSettings()->LoadConfig(file_name);
    return status;
}

int L2Window::SaveConfig(QString file_name){

    qDebug("L2Window::LoadConfig");
    getCurrentSettings()->SaveConfig(file_name);
    return status;
}

int L2Window::check(){
    QImage image;
    image = capture();
    if(image.isNull()) {
        qDebug("image.isNull()");
        findBars();
        return status;
    }
    if(status == L2_OFF || image_width != image.width() || image_height != image.height()){
        findBars();
        status = L2_OFF;
        image_width = image.width();
        image_height = image.height();
        for(int i = idCP; i < BARNUM; i++ ){
            status |= findXP(i, image);
        }


        for(int j = idCP; j < BARNUM; j++ ){
            for(int i=bar[j].getBegin();i<bar[j].getEnd();i++){
                 image.setPixel(i, bar[j].getY(), qRgb(128, 128, 128));
            }
        }


        image.save("image.png");
        QImage icotmp=image.copy(bar[idCP].getBegin()+18, bar[idCP].getY()-29, 18, 18);
    //    icotmp.save("icotmp.png");
        QPixmap pixmap(20,20);
        pixmap.convertFromImage(icotmp);
    //    pixmap.save("pixmap.png");
        if(L2icon) delete L2icon;
        L2icon = new QIcon(pixmap);

    }

    //QImage barimg=image.copy(bar[idCP].getBegin(), bar[idCP].getY(), bar[idCP].getEnd()-bar[idCP].getBegin(), 1);
    //barimg.save("CP.png");
    for(int i = idCP; i < BARNUM; i++ ){
        bar[i].checkXPBar(image);
    }
    return status;
}


int L2Window::getStatus(){return status;}

void L2Window::findBars(){
    for(int i = idCP; i < BARNUM; i++ ){
        bar[i].setStatus(false);
    }
    status = L2_OFF;
}

QString L2Window::getTitle(){
    QString title;
    QTextStream st(&title);
    st << status;
    return title;
}

QString L2Window::getNic(){
    return getCurrentSettings()->nic;
}

int L2Window::getXP(int index){return bar[index].getXP();}

QString L2Window::getConditionLabel(int index){return getCurrentSettings()->condition[index]->KeyString;}
bool L2Window::getConditionState(int index){return getCurrentSettings()->condition[index]->FSet;}
