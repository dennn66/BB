#include "xpbar.h"

XPBar::XPBar()
{
    status = BAR_OFF;
    XP = XP_ERR;
    xBegin = 0;
    xEnd = 0;
    yXP=0;
    patternMethod = false;

    //LOAD CONFIG BB.ini
    QSettings sett("bb.ini", QSettings::IniFormat);
    bEnableBarSampling = sett.value("MAIN/EnableBarSampling", 0).toBool();


}

XPBar::~XPBar()
{
    qDebug("XPBar::~XPBar");

    //statFile->close();

}

//Compare two colors with 'delta'
bool XPBar::CompareColors(QRgb color1, QRgb color2, UINT8 delta, bool mode)
{
    //qDebug("%d XPBar::CompareColors", barID);
    QColor c1(color1);
    QColor c2(color2);
    if(mode) {
        int deviation_h = (c1.hsvHue() > c2.hsvHue())? c1.hsvHue() - c2.hsvHue() : c2.hsvHue() - c1.hsvHue();
        deviation_h = (deviation_h > 179)? 360 - deviation_h : deviation_h;
        //qDebug("deviation_h: %d", deviation_h);
        int deviation_s;
        //qDebug("c1.hsvSaturation(): %d", c1.hsvSaturation());
        //qDebug("c2.hsvSaturation(): %d", c2.hsvSaturation());
        //qDebug("c1.hsvSaturation() > c2.hsvSaturation(): %d", c1.hsvSaturation() > c2.hsvSaturation());

        deviation_s = (c1.hsvSaturation() > c2.hsvSaturation())? c1.hsvSaturation() - c2.hsvSaturation() : c2.hsvSaturation() - c1.hsvSaturation();
        //qDebug("deviation_s: %d", deviation_s);

        return (deviation_h < delta) && (deviation_s < delta);
    } else {
        int deviation_r = (c1.red() > c2.red())? c1.red() - c2.red() : c2.red() - c1.red();
        int deviation_g = (c1.green() > c2.green())? c1.green() - c2.green() : c2.green() - c1.green();
        int deviation_b = (c1.blue() > c2.blue())? c1.blue() - c2.blue() : c2.blue() - c1.blue();

        return (deviation_r <delta) && (deviation_g <delta) && (deviation_b <delta);
    }
}

//Find pixel by color
bool XPBar::findPixel(QImage image, int pix_num, UINT8 pixel_delta, QRgb color, UINT8 color_delta)
{
    //qDebug("%d XPBar::findPixel", barID);
    int starty = yXP-pixel_delta;
    int endy=yXP+pixel_delta;
    starty = (starty>0)?starty:0;

    for(int row = starty; row < endy; row++){
        QRgb pix = image.pixel(QPoint(pix_num,row));
        if(CompareColors(pix, color, color_delta)){
            //qDebug("return true");
            return true;
        }
    }
    //qDebug("return false");
    return false;
}

// Calculating XP for all bars
bool XPBar::checkXPBar(QImage image){
    //qDebug("XPBar::checkXPBar");

    if(status == BAR_OFF)  {
        //qDebug("XPBar::checkXPBar bar %d is OFF", barID);
        return(false);
    }
    if(xEnd > image.width() || yXP > image.height()){
        status = BAR_OFF;
        return(false);
    }
    status = BAR_ON;


    if(patternMethod){

        int leftmargin = xBegin;
        int rightmargin = xEnd;
        int pos = xBegin;
        int postype  = checkXPPatternPartial(image, pos);
        if(postype == 0){  // color
            for(int safecounter = 0;  safecounter < 12; safecounter++){
                if(postype == 0){
                    leftmargin = pos;
                    pos = leftmargin + ((rightmargin - leftmargin - 1)*qrand())/RAND_MAX;
                } else {
                    rightmargin = pos;
                    pos = leftmargin + ((rightmargin - leftmargin - 1)*qrand())/RAND_MAX;
                }
                if((leftmargin-rightmargin)*(leftmargin-rightmargin)<5)
                {
                    XP = ((pos - xBegin)*100)/(xEnd-xBegin);
                    return true;
                }
                postype = checkXPPatternPartial(image, pos);
                if(false){

    /*
                    QDir current_dir = QDir::current();

                    QString test_file_label = "test";
                    QTextStream test_file_label_stream(&test_file_label);
                    test_file_label_stream << barID << ".csv";
                    QFile* testFile = new QFile(test_file_label);
                    QTextStream* fStream;
                    if(testFile->open(QFile::WriteOnly)){
                        fStream = new QTextStream(testFile);

                        #ifdef Q_WS_WIN
                            fStream->setCodec("Windows-1251");
                        // Под остальными ОС - utf8
                        #else
                            fStream->setCodec("utf-8");
                        #endif

                        if(fStream && fStream->device()){
                            *fStream << QDir::currentPath() << "\r\n";
                        }
                        testFile->close();

                    }
    */


                    QDir::current().mkpath("img");
                    QDir::setCurrent("img");

                    switch(barID){
                    case 4:
                        QDir::current().mkpath("HP");
                        QDir::setCurrent("HP");
                        break;
                    case 5:
                        QDir::current().mkpath("MP");
                        QDir::setCurrent("MP");
                        break;
                    default:
                        QDir::current().mkpath("other");
                        QDir::setCurrent("other");
                        break;
                    }


                    switch(postype){
                    case 0:
                        QDir::current().mkpath("color");
                        QDir::setCurrent("color");
                        break;
                    case 1:
                        QDir::current().mkpath("bkcolor");
                        QDir::setCurrent("bkcolor");
                        break;
                    default:
                        QDir::current().mkpath("none");
                        QDir::setCurrent("none");
                        break;
                    }


                    //image.setPixelColor(pos,yXP, QColor(128, 128, 128));

     //               sample = image.copy(pos-10,yXP-2,20,5);
     //               sample.setPixelColor(9,0, QColor(255, 255, 255));
     //               sample.setPixelColor(11,0, QColor(255, 255, 255));
     //               sample.setPixelColor(9,4, QColor(255, 255, 255));
     //               sample.setPixelColor(11,4, QColor(255, 255, 255));
                    QImage sample;

                    sample = image.copy(xBegin,yXP-2,xEnd - xBegin,5);
                    int x1 = pos - xBegin - 1;
                    int x2 = x1 + 2;
                    if(x1 > 0){
                        sample.setPixelColor(x1 ,0, QColor(255, 255, 255));
                        sample.setPixelColor(x1 ,4, QColor(255, 255, 255));
                    }
                    if(x2 <  sample.width()){
                        sample.setPixelColor(x2 ,0, QColor(255, 255, 255));
                        sample.setPixelColor(x2 ,4, QColor(255, 255, 255));
                    }

                    QString file_label = "i";
                    QTextStream file_label_stream(&file_label);
                    int fnum = qrand();
                    file_label_stream  << fnum << ".png";


                    sample.save(file_label);
                    barpattern.save("0pattern.png");

                    QDir::setCurrent("..");
                    QDir::setCurrent("..");
                    QDir::setCurrent("..");

                }
            }
            XP = ((leftmargin - xBegin)*100)/(xEnd-xBegin);
        } else {   //bkcolor or err
            int grey_c[5];
            for(int i = -2; i < 3; i++ ){
                QColor img_pix = image.pixelColor(QPoint(pos,yXP+i));
                grey_c[i+2] = (2126*img_pix.red()+7152*img_pix.green()+722*img_pix.blue())/10000;
            }
            bool dif01 = ((grey_c[1] - grey_c[0]) > 7) || ((grey_c[1] - grey_c[0]) < -8);
            bool dif12 = (grey_c[2] - grey_c[1]) < -8;
            bool dif23 = (grey_c[3] - grey_c[2]) > 3;

            if(dif01 && dif12 && dif23){
                XP = 0; //bkcolor
            } else {
                XP = XP_ERR;  //err
            }
        }
        bool err1 = ((postype == 1) && (XP == XP_ERR));
        bool err2 = ((postype == -1) && (XP == 0));
        if(err1) postype = -1;
        if(err2) postype =  1;

        if((bEnableBarSampling) && (qrand() < 100 || (err1 && (barID == 4)) || err2)){
            QDir::current().mkpath("img");
            QDir::setCurrent("img");

            switch(barID){
            case 4:
                QDir::current().mkpath("HP");
                QDir::setCurrent("HP");
                break;
            case 5:
                QDir::current().mkpath("MP");
                QDir::setCurrent("MP");
                break;
            default:
                QDir::current().mkpath("other");
                QDir::setCurrent("other");
                break;
            }


            switch(postype){
            case 0:
                QDir::current().mkpath("color");
                QDir::setCurrent("color");
                break;
            case 1:
                QDir::current().mkpath("bkcolor");
                QDir::setCurrent("bkcolor");
                break;
            default:
                QDir::current().mkpath("none");
                QDir::setCurrent("none");
                break;
            }


            //image.setPixelColor(pos,yXP, QColor(128, 128, 128));

            QImage sample;

            sample = image.copy(xBegin,yXP-2,xEnd - xBegin,5);
            int x1 = pos - xBegin - 1;
            int x2 = x1 + 2;
            if(x1 > 0){
                sample.setPixelColor(x1 ,0, QColor(255, 255, 255));
                sample.setPixelColor(x1 ,4, QColor(255, 255, 255));
            }
            if(x2 <  sample.width()){
                sample.setPixelColor(x2 ,0, QColor(255, 255, 255));
                sample.setPixelColor(x2 ,4, QColor(255, 255, 255));
            }

            QString file_label = "i";
            if(err1 || err2) file_label = "e";
            QTextStream file_label_stream(&file_label);
            int fnum = qrand();
            file_label_stream  << fnum << ".png";


            sample.save(file_label);
            barpattern.save("0pattern.png");

            QDir::setCurrent("..");
            QDir::setCurrent("..");
            QDir::setCurrent("..");

            int grey_c[5];
            for(int i = -2; i < 3; i++ ){
                QColor img_pix = image.pixelColor(QPoint(pos,yXP+i));
                grey_c[i+2] = (2126*img_pix.red()+7152*img_pix.green()+722*img_pix.blue())/10000;
            }
            QString test_file_label = "xBegin";
            QTextStream test_file_label_stream(&test_file_label);
            test_file_label_stream << barID << " " << postype << ".csv";
            QFile* testFile = new QFile(test_file_label);
            QTextStream* fStream;
            if(testFile->open(QFile::WriteOnly| QIODevice::Append)){
                fStream = new QTextStream(testFile);
                #ifdef Q_WS_WIN
                    fStream->setCodec("Windows-1251");
                // Под остальными ОС - utf8
                #else
                    fStream->setCodec("utf-8");
                #endif

                if(fStream && fStream->device()){
                    for(int i = 0; i < 5; i++){
                        *fStream << grey_c[i] << ";";
                    }
                    *fStream << file_label << ";"<< err1 << ";"<< err2 << "\r\n";
                }
                testFile->close();
            }
        }
    } else {
        XP = checkXPBarPartial(image, xBegin, xEnd);
    }
    return(true);
}

//Compare pixels at 'pos' position with pattern
int XPBar::checkXPPatternPartial(QImage image, int pos){
    int valid=0;

    for(int j = 0; j < 2; j++ ){
        valid = 0;
        for(int i = -1; i < 2; i++ ){
            QColor img_pix = image.pixelColor(QPoint(pos,yXP+i));
            QColor pattern_pix = barpattern.pixelColor(QPoint(j, 2+i));
            int deviation_r = (img_pix.red() > pattern_pix.red())? img_pix.red() - pattern_pix.red() : pattern_pix.red() - img_pix.red();
            int deviation_g = (img_pix.green() > pattern_pix.green())? img_pix.green() - pattern_pix.green() : pattern_pix.green() - img_pix.green();
            int deviation_b = (img_pix.blue() > pattern_pix.blue())? img_pix.blue() - pattern_pix.blue() : pattern_pix.blue() - img_pix.blue();
            if( (deviation_r <3+j*2) && (deviation_g <3+j*2) && (deviation_b <3+j*2)) valid++;
            if(valid > 0)  return j;
        }
    }
    return -1;
}

//submethod for calculating XP for HPCPMP
int XPBar::checkXPBarPartial(QImage image, int begin, int end){
    int barsize = end-begin;
    int max = 0;
    int position = begin;
    int counter = 0;
    int validator = 0;

    for(int j=begin;j<end;j++) {
        if(findPixel(image, j, 1, barcolor, 5)){
            counter++;
            validator++;
            max = (counter > max)? counter: max;
            position = (counter == max)? j:position;
        } else if(findPixel(image, j, 1, barbkclr, 5)){
            counter--;
            validator++;
            max = (counter > max)? counter: max;
            position = (counter == max)? j:position;
        }
    }
    position = (position <begin )? begin:position;
    position = (position >end )? end:position;

   return (validator < 2)?XP_ERR:((position- begin)*100)/barsize;
}

// Compare random points between image and pattern
bool XPBar::checkPattern(QPoint topleft, QImage* image, QImage* pattern, int num, int treshold, int deviation){
    if(!(topleft.rx() > 0 && topleft.ry() > 0)) return false;
    int         notvalid = 0;
    for(int i=0; i<num; i ++)
    {
        int x = (pattern->width()*qrand())/RAND_MAX;
        int y = (pattern->height()*qrand())/RAND_MAX;
        QColor img_pix = image->pixelColor(QPoint(x+topleft.rx(),y+topleft.ry()));
        QColor pattern_pix = pattern->pixelColor(QPoint(x,y));
        if(!(pattern_pix.red() == 255 && pattern_pix.green() == 255 &&pattern_pix.blue() == 255)){
            int deviation_r = (img_pix.red() > pattern_pix.red())? img_pix.red() - pattern_pix.red() : pattern_pix.red() - img_pix.red();
            int deviation_g = (img_pix.green() > pattern_pix.green())? img_pix.green() - pattern_pix.green() : pattern_pix.green() - img_pix.green();
            int deviation_b = (img_pix.blue() > pattern_pix.blue())? img_pix.blue() - pattern_pix.blue() : pattern_pix.blue() - img_pix.blue();
            if( (deviation_r >deviation) || (deviation_g >deviation) || (deviation_b >deviation)) notvalid++;
            if(notvalid > treshold)  return false;
        }
    }
    return true;
}

bool XPBar::getStatus(){return status;}

int XPBar::getXP(){return (status == BAR_ON)? XP : XP_ERR;}

void XPBar::setColors(QRgb color, QRgb bk_color){
    barcolor = color;
    barbkclr = bk_color;
}

int XPBar::getY(){return yXP;}
int XPBar::getBegin(){return xBegin;}
int XPBar::getEnd(){return xEnd;}
void XPBar::setbarID(int id){barID = id;}
void XPBar::setStatus(bool stt){ status = stt;}
