#include "xpbar.h"

XPBar::XPBar()
{
    status = BAR_OFF;
    XP = XP_ERR;
    xBegin = 0;
    xEnd = 0;
    yXP=0;
    patternMethod = false;

}


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
    if(patternMethod){
        int leftmargin = xBegin;
        int rightmargin = xEnd;
        int pos = xEnd;
        int postype = -1;
        int counter = 0;
        int validator = 0;

        for(int safecounter = 0;  safecounter < 50; safecounter++){
            switch(postype){
                case 0:     // color
                    leftmargin = pos;
                    pos = leftmargin + ((rightmargin - leftmargin - 1)*qrand())/RAND_MAX;
                    break;
                case 1:  //bkcolor
                    rightmargin = pos;
                    pos = leftmargin + ((rightmargin - leftmargin - 1)*qrand())/RAND_MAX;
                    break;
                default:  //err
                    pos = leftmargin + ((pos - leftmargin - 1)*qrand())/RAND_MAX;
                    break;
            }

            postype = checkXPPatternPartial(image, pos);
            if(postype != -1) validator++;
            counter++;

            if(counter > 20 && validator < 2){
                XP = XP_ERR;
                return true;
            }

            if((leftmargin-rightmargin)*(leftmargin-rightmargin)<10)
            {
                XP = ((pos - xBegin)*100)/(xEnd-xBegin);
                return true;
            }
        }
        XP = XP_ERR;
        return true;
    } else {
        XP = checkXPBarPartial(image, xBegin, xEnd);
    }
    status = BAR_ON;
    return(true);
}

int XPBar::checkXPPatternPartial(QImage image, int pos){
    static bool done0 = true;
    static bool done1 = true;
    int valid=0;


    //QImage* test = new QImage(QSize(3,5), QImage::Format_RGB32);
    if(!done0){
        QImage tmp = image.copy(QRect(xBegin,yXP,xEnd - xBegin, barpattern.height()));
        done0 = true;
        tmp.save("test_bar0.bmp");
        QFile* statFile = new QFile("test_bar0.csv");

        if(statFile->open(QFile::WriteOnly)) {
            QTextStream* stStream = new QTextStream(statFile);

            #ifdef Q_WS_WIN
            stStream->setCodec("Windows-1251");
            // Под остальными ОС - utf8
            #else
            stStream->setCodec("utf-8");
            #endif

            // Запись заголовка с информацией о запуске
            if(stStream && stStream->device())
            {
                *stStream <<  "x"  << ";" << "y" << ";" << "pix.red" << ";" << "pix.green" << ";" << "pix.blue" << "\r\n";
                for(int y = 0; y<tmp.height();y++){
                    for(int x = 0; x < tmp.width(); x++){
                        QColor pix = tmp.pixelColor(QPoint(x,y));
                        *stStream << x  << ";" << y << ";" << pix.red() << ";" << pix.green() << ";" <<pix.blue() << "\r\n";
                    }
                }
            }

            statFile->close();
        }

    } else     if(!done1){
        QImage tmp = image.copy(QRect(xBegin,yXP,xEnd - xBegin, barpattern.height()));
        done1 = true;
        tmp.save("test_bar1.bmp");
    }

    for(int j = 0; j < 2; j++ ){
        valid = 0;
        for(int i = 1; i < barpattern.height()-1; i++ ){
            QColor img_pix = image.pixelColor(QPoint(pos,yXP+i));
            QColor pattern_pix = barpattern.pixelColor(QPoint(j, i));
            int deviation_r = (img_pix.red() > pattern_pix.red())? img_pix.red() - pattern_pix.red() : pattern_pix.red() - img_pix.red();
            int deviation_g = (img_pix.green() > pattern_pix.green())? img_pix.green() - pattern_pix.green() : pattern_pix.green() - img_pix.green();
            int deviation_b = (img_pix.blue() > pattern_pix.blue())? img_pix.blue() - pattern_pix.blue() : pattern_pix.blue() - img_pix.blue();
            if( (deviation_r <5) && (deviation_g <5) && (deviation_b <5)) valid++;
            if(valid > 0)  return j;
        }
    }
    //qDebug(" XPBar::checkXPPatternPartial(QImage image, int pos) bar %d is OFF J=all pos=%d", barID,  pos);
    return -1;
}
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

bool XPBar::checkPattern(QPoint topleft, QImage* image, QImage* pattern, int num, int treshold, int deviation){
    if(!(topleft.rx() > 0 && topleft.ry() > 0)) return false;
    int         notvalid = 0;
    for(int i=0; i<num; i ++)
    {
        int x = (pattern->width()*qrand())/RAND_MAX;
        int y = (pattern->height()*qrand())/RAND_MAX;
        QColor img_pix = image->pixelColor(QPoint(x+topleft.rx(),y+topleft.ry()));
        if(!(img_pix.red() == 255 && img_pix.green() == 255 &&img_pix.blue() == 255)){
            QColor pattern_pix = pattern->pixelColor(QPoint(x,y));
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
