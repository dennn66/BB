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
    image_width = 0;
    image_height = 0;
    L2icon = NULL;
    bStar = false;

    mainleft.load("patterns\\main_left.bmp");
    mainright.load("patterns\\main_right.bmp");
    mobleft_o.load("patterns\\mob_left_open.bmp");
    mobright_o.load("patterns\\mob_right_open.bmp");
    mobleft_c.load("patterns\\mob_left_close.bmp");
    mobright_c.load("patterns\\mob_right_close.bmp");
    star.load("patterns\\star.bmp");

    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);
    bEnableToolbar = sett.value("MAIN/EnableToolbar", 0).toBool();

    toolbarleft.load("patterns\\toolbar_left.bmp");


    resetBars();

    bar[idCP].setColors(CP_COLOR, BK_CP_COLOR);
    bar[idHP].setColors(HP_COLOR, BK_HP_COLOR);
    bar[idMP].setColors(MP_COLOR, BK_MP_COLOR);
    bar[idVP].setColors(VP_COLOR, BK_VP_COLOR);
    bar[idMobHP].setColors(MobHP_COLOR, BK_MobHP_COLOR);
    bar[idMobMP].setColors(MobMP_COLOR, BK_MobMP_COLOR);

    for(int i = idCP; i < BARNUM; i++ ){
        bar[i].setbarID(i);
    }

    KeyConditionsSet* conditionSet = new KeyConditionsSet();
    cond_set_list.append(conditionSet);
    project_file_name = "default.bbproj";
    activeCondSet = 0;



}

QImage L2Window::capture(){
    QImage image;
    qDebug("L2Window::capture");

    if(IsIconic(hwnd) != 0){
        qDebug("Window is iconic: %d", (int) hwnd);
        windowtopright.setX(0);
        windowtopright.setY(0);
        isActiveWindow = false;
        return image;
    }


    RECT windowRect;
    GetWindowRect(hwnd, &windowRect);

    HWND foreground = GetForegroundWindow();
    if(foreground == hwnd) {
        qDebug("Window is foreground: %d", (int) hwnd);
        windowtopright.setX(windowRect.right);
        windowtopright.setY(windowRect.top);
        isActiveWindow = true;
    } else {
        qDebug("Window is background: %d", (int) hwnd);
        windowtopright.setX(windowRect.right);
        windowtopright.setY(windowRect.top);
        isActiveWindow = false;
    }

    HDC hdcSrc = GetWindowDC(hwnd);
    if(hdcSrc == NULL){
        qDebug("GetDC failed: %d", (int) hwnd);
        return image;
    }



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
        ReleaseDC(hwnd, hdcSrc);
        QPixmap pixelMap = QtWin::fromHBITMAP(hBitmap);
        DeleteObject(hBitmap);
        //pixelMap.load("C:\\workspace\\shots\\Lineage - self - star.bmp");
        //pixelMap.load("C:\\workspace\\shots\\Vred.bmp");
       // pixelMap.save("Lineage.png");
        image = pixelMap.toImage();
    } else {
        ReleaseDC(hwnd, hdcSrc);
    }
    return image;
}


QIcon* L2Window::getIcon(){
//   qDebug("L2Window::getIcon");
    return L2icon;
}

bool L2Window::getStarState(){
//   qDebug("L2Window::getIcon");
    return bStar;
}

QColor* L2Window::getStarColor(){
//   qDebug("L2Window::getIcon");
    return &c2;
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

QImage* L2Window::getTool(int n){
    qDebug("L2Window::getTool(int n) %d", n);
    return &tool[n];
}

QPoint L2Window::findPattern(QImage source, QPoint topleft, QPoint bottomright, QImage pattern, int delta){
    qDebug("L2Window::findPattern");
    if(topleft.rx() <0 ) topleft.setX(0);
    if(topleft.ry() <0 ) topleft.setY(0);
    if(bottomright.rx() < topleft.rx() ) bottomright.setX(topleft.rx());
    if(bottomright.ry() < topleft.ry() ) bottomright.setY(topleft.ry());

    for(int im_w = topleft.rx(); im_w < bottomright.rx(); im_w++ ) {
        for(int im_h = topleft.ry(); im_h < bottomright.ry(); im_h++ ) {

            int count_total = 0;
            int count_match = 0;
            for(int w = 0; w < pattern.width();w++){
                for(int h = 0; h < pattern.height();h++){
                    //qDebug("Read pattern");
                    QRgb frame_pix = pattern.pixel(QPoint(w, h));
                    QColor ct(frame_pix);
                    //qDebug("Color %d %d %d", ct.red(), ct.green(), ct.blue());
                    if(!(ct.red() == 255 && ct.green()==255 && ct.blue() == 255)){
                        count_total++;
                        //qDebug("Read frame");
                         QRgb image_pix = source.pixel(QPoint(im_w+w, im_h+h));
                         bool res = XPBar::CompareColors(frame_pix,image_pix, 5);
                         count_match += res;
                    }
                    if(count_total - count_match >= delta) break;
                }
                if(count_total - count_match >= delta) break;
            }
           // qDebug("%d %d findPattern = %d", im_w, im_h, count_total - count_match);
            if(count_total - count_match < delta){
                for(int w = 0; w < pattern.width();w++){
                    for(int h = 0; h < pattern.height();h++){

                        source.setPixel(im_w+w, im_h+h, qRgb(128, 128, 128));
                    }
                }
             //   source.save("source.png");
                return(QPoint(im_w, im_h));

            }
        }
    }
    return(QPoint(-1,-1));
}

int L2Window::check(){
    QImage image;
    image = capture();
    static bool done = FALSE;
    bool invalidmain;
    bool invalidmob;



#define INVALIDMAIN (maintopleft.rx()< 0 || maintopright.rx()< 0 )
#define INVALIDMOB (mobtopleft.rx()<0 || mobtopright.rx()< 0 )

    if(image.isNull()) {
        qDebug("image.isNull()");
        resetBars();
        return status;
    }
    if(status == L2_OFF || image_width != image.width() || image_height != image.height()){
        resetBars();
        image_width = image.width();
        image_height = image.height();
    }

    invalidmain = (maintopleft.rx()< 0 || maintopright.rx()< 0 );
    invalidmob  = (mobtopleft.rx()<0 || mobtopright.rx()< 0 );

    if(maintopleft.rx()<0){
        maintopleft = findPattern(image, QPoint(0, 0), QPoint(20,50), mainleft, 5);
        qDebug("maintopleft %d %d", maintopleft.rx(), maintopleft.ry());
        //image.save("image.png");
        if(maintopleft.rx()>0){
            QImage tmp = image.copy(maintopleft.rx(), maintopleft.ry(), mainleft.width(), mainleft.height());
            for(int x = 0; x < mainleft.width(); x++){
                for(int y = 0; y < mainleft.height(); y++){
                    QRgb current_pix = tmp.pixel(QPoint(x, y));
                    QRgb pattern_pix = mainleft.pixel(QPoint(x, y));



                    QColor ct(pattern_pix);
                    if(!(ct.red() == 255 && ct.green()==255 && ct.blue() == 255)){
                        if(current_pix != pattern_pix){
                            tmp.setPixel(x, y, RGB(0,255,0));
                        }
                    }
                }
            }
            tmp.save("mainleft_new.png");
        }
    }
    if(maintopright.rx()<0){
        maintopright = findPattern(image, QPoint(maintopleft.rx()+50, maintopleft.ry()-10), QPoint(maintopleft.rx()+390, maintopleft.ry()+10), mainright, 5);
        qDebug("maintopright %d %d", maintopright.rx(), maintopright.ry());
        if(maintopright.rx()>0){
            QImage tmp = image.copy(maintopright.rx(), maintopright.ry(), mainright.width(), mainright.height());
            for(int x = 0; x < mainright.width(); x++){
                for(int y = 0; y < mainright.height(); y++){
                    QRgb current_pix = tmp.pixel(QPoint(x, y));
                    QRgb pattern_pix = mainright.pixel(QPoint(x, y));



                    QColor ct(pattern_pix);
                    if(!(ct.red() == 255 && ct.green()==255 && ct.blue() == 255)){
                        if(current_pix != pattern_pix){
                            tmp.setPixel(x, y, RGB(0,255,0));
                        }
                    }
                }
            }
            tmp.save("mainright_new.png");
        }
    }
#define TOOLLOFFSET 38
#define TOOLTOFFSET 9
#define TOOLVGAP 14
#define TOOLHGAP 5
#define TOOLH2GAP 7
#define TOOLSELL 32
    if(toolbartopleft.rx()<0 && bEnableToolbar && (tooldetectcounter++ > 20)){
        tooldetectcounter = 0;
        toolbartopleft = findPattern(image, QPoint(image.width()/4, image.height()/2), QPoint(image.width()-521,image.height()-200), toolbarleft, 30);
        qDebug("toolbartopleft %d %d", toolbartopleft.rx(), toolbartopleft.ry());
        if((toolbartopleft.rx()>0 && toolbartopleft.ry()> 0 )) {


            //image.save("image.png");
            //QImage icotmp=image.copy(toolbartopleft.rx(), toolbartopleft.ry(), 521, 237);
            //icotmp.save("toolbar.png");
            for(int k=1;k<5;k++){
                for(int l=0;l<3;l++){
                    for(int m=0;m<4;m++){

                        QString fname;
                        QTextStream varstream(&fname);
                        fname = "tool";
                        varstream  <<(4-k)*12+l*4 + m << ".png";
                        // 1 1
                        int sell_x = TOOLLOFFSET + (l*4 + m)*TOOLSELL + (l*4 + m)*TOOLHGAP + l*TOOLH2GAP;
                        int sell_y = TOOLTOFFSET + k*TOOLSELL + k*TOOLVGAP;
                        qDebug("sell %s %d %d", fname.toStdString().c_str(), sell_x, sell_y);

                        tool[(4-k)*12+l*4 + m]=image.copy(toolbartopleft.rx()+sell_x, toolbartopleft.ry()+sell_y, TOOLSELL, TOOLSELL);
                        //tool[(4-k)*12+l*4 + m].save(fname);
                    }
                }
            }
        }

    }

    if(!INVALIDMAIN){

        if(invalidmain){
            QImage icotmp=image.copy(maintopleft.rx()+40, maintopleft.ry()+3, 18, 18);
            //icotmp.save("icotmp.png");
            QPixmap pixmap(20,20);
            pixmap.convertFromImage(icotmp);
        //    pixmap.save("pixmap.png");
            if(L2icon) delete L2icon;
            L2icon = new QIcon(pixmap);
            for(int i = idCP; i <= idVP; i++ ){
                bar[i].xBegin = maintopleft.rx()+19;
                bar[i].xEnd = maintopright.rx()-2;
                bar[i].yXP = maintopleft.ry()+27+(i-idCP)*13+5;
                bar[i].setStatus(true);
            }
        }
        if(mobtopleft.rx()<0 && (mobdetectcounter++ > 20)){
            mobdetectcounter = 0;
            mobtopleft = findPattern(image, QPoint(maintopright.rx(), 0), QPoint(image_width/2,50), mobleft_c, 5);
            if(mobtopleft.rx()<0 ){
                mobtopleft = findPattern(image, QPoint(maintopright.rx(), 0), QPoint(image_width/2,50), mobleft_o, 5);
                qDebug("mobtopleft open %d %d", mobtopleft.rx(), mobtopleft.ry());
                if(mobtopleft.rx()>0){
                    QImage tmp = image.copy(mobtopleft.rx(), mobtopleft.ry(), mobleft_o.width(), mobleft_o.height());
                    for(int x = 0; x < mobleft_o.width(); x++){
                        for(int y = 0; y < mobleft_o.height(); y++){
                            QRgb current_pix = tmp.pixel(QPoint(x, y));
                            QRgb pattern_pix = mobleft_o.pixel(QPoint(x, y));
                            QColor ct(pattern_pix);
                            if(!(ct.red() == 255 && ct.green()==255 && ct.blue() == 255)){
                                if(current_pix != pattern_pix){
                                    tmp.setPixel(x, y, RGB(0,255,0));
                                }
                            }
                        }
                    }
                    tmp.save("mobleft_o_new.png");
                }
            } else {
                if(mobtopleft.rx()>0){
                    QImage tmp = image.copy(mobtopleft.rx(), mobtopleft.ry(), mobleft_c.width(), mobleft_c.height());
                    for(int x = 0; x < mobleft_c.width(); x++){
                        for(int y = 0; y < mobleft_c.height(); y++){
                            QRgb current_pix = tmp.pixel(QPoint(x, y));
                            QRgb pattern_pix = mobleft_c.pixel(QPoint(x, y));
                            QColor ct(pattern_pix);
                            if(!(ct.red() == 255 && ct.green()==255 && ct.blue() == 255)){
                                if(current_pix != pattern_pix){
                                    tmp.setPixel(x, y, RGB(0,255,0));
                                }
                            }
                        }
                    }
                    tmp.save("mobleft_c_new.png");
                }
                qDebug("mobtopleft close %d %d", mobtopleft.rx(), mobtopleft.ry());
            }
        }

        if(mobtopright.rx()<0 && mobtopleft.rx()>0){
            mobtopright = findPattern(image, QPoint(mobtopleft.rx()+160, mobtopleft.ry()-1), QPoint(mobtopleft.rx()+390, mobtopleft.ry()+1), mobright_c, 5);
            if(mobtopright.rx()<0 ){
                mobtopright = findPattern(image, QPoint(mobtopleft.rx()+160, mobtopleft.ry()-1), QPoint(mobtopleft.rx()+390, mobtopleft.ry()+1), mobright_o, 5);
                qDebug("mobtopright open %d %d", mobtopright.rx(), mobtopright.ry());
                if(mobtopright.rx()>0){
                    QImage tmp = image.copy(mobtopright.rx(), mobtopright.ry(), mobright_o.width(), mobright_o.height());
                    for(int x = 0; x < mobright_o.width(); x++){
                        for(int y = 0; y < mobright_o.height(); y++){
                            QRgb current_pix = tmp.pixel(QPoint(x, y));
                            QRgb pattern_pix = mobright_o.pixel(QPoint(x, y));
                            QColor ct(pattern_pix);
                            if(!(ct.red() == 255 && ct.green()==255 && ct.blue() == 255)){
                                if(current_pix != pattern_pix){
                                    tmp.setPixel(x, y, RGB(0,255,0));
                                }
                            }
                        }
                    }
                    tmp.save("mobright_o_new.png");
                }
            } else {
                qDebug("mobtopright close %d %d", mobtopright.rx(), mobtopright.ry());
                if(mobtopright.rx()>0){
                    QImage tmp = image.copy(mobtopright.rx(), mobtopright.ry(), mobright_c.width(), mobright_c.height());
                    for(int x = 0; x < mobright_c.width(); x++){
                        for(int y = 0; y < mobright_c.height(); y++){
                            QRgb current_pix = tmp.pixel(QPoint(x, y));
                            QRgb pattern_pix = mobright_c.pixel(QPoint(x, y));
                            QColor ct(pattern_pix);
                            if(!(ct.red() == 255 && ct.green()==255 && ct.blue() == 255)){
                                if(current_pix != pattern_pix){
                                    tmp.setPixel(x, y, RGB(0,255,0));
                                }
                            }
                        }
                    }
                    tmp.save("mobright_c_new.png");
                }

            }
        }

        if(mobtopleft.rx()>0 && mobtopright.rx()>0 ){
            //QImage mobbar = image.copy(mobtopleft.rx(), mobtopleft.ry(), mobtopright.rx() - mobtopleft.rx(), mobleft.height());
            //mobbar.save("mobbar.png");
            if(invalidmob){
                for(int i = idMobHP; i <= idMobMP; i++ ){
                    bar[i].xBegin = mobtopleft.rx()+20;
                    bar[i].xEnd = mobtopright.rx()+11;
                    bar[i].yXP = mobtopleft.ry()+28+(i-idMobHP)*8;
                    bar[i].setStatus(true);
                }
            }
            if((startopleft.rx()<0 && (stardetectcounter++ < 20))  || invalidmain||invalidmob){
                stardetectcounter = 0;
                startopleft = findPattern(image, QPoint(bar[idMobHP].getBegin(), bar[idMobHP].getY()-23), QPoint(bar[idMobHP].getBegin()+20, bar[idMobHP].getY()-5), star, 5);
                qDebug("startopleft %d %d", startopleft.rx(), startopleft.ry());
                if(startopleft.rx()>0){
                    QImage tmp = image.copy(startopleft.rx(), startopleft.ry(), star.width(), star.height());
                    for(int x = 0; x < star.width(); x++){
                        for(int y = 0; y < star.height(); y++){
                            QRgb current_pix = tmp.pixel(QPoint(x, y));
                            QRgb pattern_pix = star.pixel(QPoint(x, y));
                            QColor ct(pattern_pix);
                            if(!(ct.red() == 255 && ct.green()==255 && ct.blue() == 255)){
                                if(current_pix != pattern_pix){
                                    tmp.setPixel(x, y, RGB(0,255,0));
                                }
                            }
                        }
                    }
                    tmp.save("star_new.png");
                }
            }
        }

    }

    status = L2_ON;
    if(!INVALIDMAIN && !done && false){

        QImage frames=image.copy(0, 0, image.width(), image.height());

        for(int i=maintopleft.rx();i<maintopright.rx();i++){
                 //frames.setPixel(i, maintopleft.ry, qRgb(255, 0, 0));
        }
        for(int j = idCP; j <= idVP; j++ ){
            for(int i=bar[j].getBegin();i<bar[j].getEnd();i++){
                     frames.setPixel(i, bar[j].getY(), qRgb(128, 128, 128));
            }
        }
        if(!INVALIDMOB){
            for(int i=mobtopleft.rx();i<mobtopright.rx();i++){
                     //frames.setPixel(i, mobtopleft.ry, qRgb(0, 255, 0));
            }

            for(int j = idMobHP; j <= idMobMP; j++ ){
                for(int i=bar[j].getBegin();i<bar[j].getEnd();i++){
                         frames.setPixel(i, bar[j].getY(), qRgb(128, 128, 128));
                }
            }
        }
        frames.save("frames.png");
        done = TRUE;
    }



    //QImage barimg=image.copy(bar[idCP].getBegin(), bar[idCP].getY(), bar[idCP].getEnd()-bar[idCP].getBegin(), 1);
    //barimg.save("CP.png");

    for(int i = idCP; i < BARNUM; i++ ){
        bar[i].checkXPBar(image);
    }

    if(bar[idMobMP].getXP() != XP_ERR) {
        targettype = TARGETMEORPET;
    } else if(bar[idMobHP].getXP() != XP_ERR){
        targettype = TARGETMOB;
    } else {
        //QImage mobbar = image.copy(mobtopleft.rx()-1, mobtopleft.ry()-1, mobleft.width()+2, mobleft.height()+2);
        //mobbar.save("mobbar.png");

        QPoint p1 = findPattern(image, QPoint(mobtopleft.rx()-1, mobtopleft.ry()-1), QPoint(mobtopleft.rx()+1, mobtopleft.ry()+1), mobleft_c, 5);
        if(p1.rx() < 0) p1 = findPattern(image, QPoint(mobtopleft.rx()-1, mobtopleft.ry()-1), QPoint(mobtopleft.rx()+1, mobtopleft.ry()+1), mobleft_o, 5);
        if(p1.rx() < 0) p1 = findPattern(image, QPoint(mobtopright.rx()-1, mobtopright.ry()-1), QPoint(mobtopright.rx()+1, mobtopright.ry()+1), mobright_c, 5);
        if(p1.rx() < 0) p1 = findPattern(image, QPoint(mobtopright.rx()-1, mobtopright.ry()-1), QPoint(mobtopright.rx()+1, mobtopright.ry()+1), mobright_o, 5);

        if(p1.rx() > 0){
            targettype = TARGETCHAR;
        } else {
            targettype = NOTARGET;
        }
        //targettype = p.rx() ;
    }

    if(startopleft.rx() > 0 && ((targettype == TARGETCHAR)||(targettype == TARGETMOB))){
        QImage icotmp2=image.copy(startopleft.rx()+3, startopleft.ry()+4, 6, 6);
     //   icotmp2.save("icotmp2.png");
        for(int j=0;j<6;j++) {
            for(int i=0;i<6;i++){
                 QRgb pix = icotmp2.pixel(QPoint(i,j));
                 QColor c1(pix);
                 if((j == 0) && (i == 0)) c2=c1;
                 c2.setRed((c2.red()+c1.red())/2);
                 c2.setGreen((c2.green()+c1.green())/2);
                 c2.setBlue((c2.blue()+c1.blue())/2);
            }
        }
        bStar = XPBar::CompareColors(c2.rgb(),STAR_COLOR, 5);
    } else {
        c2.setRgb(qRgb(128, 128, 128));
        bStar = false;
    }

    for(int i = 0; i < KEYNUM; i++){

        if((toolbartopleft.rx()>0 && toolbartopleft.ry()> 0 )) {


            //image.save("image.png");
            //QImage icotmp=image.copy(toolbartopleft.rx(), toolbartopleft.ry(), 521, 237);
            //icotmp.save("toolbar.png");
            for(int k=1;k<5;k++){
                for(int l=0;l<3;l++){
                    for(int m=0;m<4;m++){
                        if(getConditionSkill((4-k)*12+l*4 + m) && getConditionState((4-k)*12+l*4 + m)){
                            //QString fname;
                            //QTextStream varstream(&fname);
                            //fname = "tool";
                            //varstream  <<(4-k)*12+l*4 + m << ".png";
                            // 1 1
                            int sell_x = TOOLLOFFSET + (l*4 + m)*TOOLSELL + (l*4 + m)*TOOLHGAP + l*TOOLH2GAP;
                            int sell_y = TOOLTOFFSET + k*TOOLSELL + k*TOOLVGAP;
                            //qDebug("sell %s %d %d", fname.toStdString().c_str(), sell_x, sell_y);
                            //tool[(4-k)*12+l*4 + m].save(fname);

                            bool something_is_not_the_same=false;

                            for(int i=0; i<TOOLSELL; i += 8)
                            {
                                if(something_is_not_the_same) break;
                                for(int j=0; j<TOOLSELL; j += 8)
                                {
                                    if(QColor(tool[(4-k)*12+l*4 + m].pixel(i,j)).name()!=QColor(image.pixel(toolbartopleft.rx()+sell_x+i,toolbartopleft.ry()+sell_y+j)).name())
                                    {
                                        something_is_not_the_same=true;
                                        break;
                                    }
                                }
                            }
                            skillrdy[(4-k)*12+l*4 + m] = !something_is_not_the_same;
                        }
                    }
                }
            }
        }

    }



    return status;
}


int L2Window::getStatus(){return status;}

void L2Window::resetBars(){
    for(int i = idCP; i < BARNUM; i++ ){
        bar[i].setStatus(false);
    }
    maintopleft = QPoint(-1, -1);
    maintopright = QPoint(-1, -1);
    mobtopleft = QPoint(-1, -1);
    mobtopright = QPoint(-1, -1);
    startopleft = QPoint(-1, -1);
    toolbartopleft  = QPoint(-1, -1);
    mobdetectcounter = 0;
    stardetectcounter = 0;
    tooldetectcounter = 0;

    status = L2_OFF;
    bStar = false;
    targettype = NOTARGET;
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
