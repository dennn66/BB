#include "xpbar.h"

XPBar::XPBar()
{
    status = BAR_OFF;
    XP = XP_ERR;
    xBegin = 0;
    xEnd = 0;
    yXP=0;

}


bool XPBar::CompareColors(QRgb color1, QRgb color2, UINT8 delta)
{
    //qDebug("%d XPBar::CompareColors", barID);
    QColor c1(color1);
    QColor c2(color2);

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
}

bool XPBar::findPixel(QImage image, int pix_num, UINT8 pixel_delta, QRgb color, UINT8 color_delta)
{
    //qDebug("%d XPBar::findPixel", barID);
//    int startx = pix_num-pixel_delta;
//    int endx = pix_num+pixel_delta;
    int starty = yXP-pixel_delta;
    int endy=yXP+pixel_delta;
 //   startx = (startx>0)?startx:0;
    starty = (starty>0)?starty:0;
    //qDebug("startx: %d starty: %d", startx, starty);

    for(int row = starty; row < endy; row++){
 //       for(int col = startx; col < endx; col++){
            //qDebug("%d -> (col: %d row: %d)", barID, col, row);
            QRgb pix = image.pixel(QPoint(pix_num,row));
            if(CompareColors(pix, color, color_delta)){
                //qDebug("return true");
                return true;
            }
//        }
    }
    //qDebug("return false");
    return false;
}
/*
bool XPBar::checkXPBar(QImage image){
    //qDebug("XPBar::checkXPBar");
  int barsize = xEnd-xBegin;
  int max = 0;
  int position = xBegin;
  int counter = 0;
  int validator = 0;
    if(status == BAR_OFF)  {
      //  qDebug("XPBar::checkXPBar bar %d is OFF", barID);
        return(false);
    }
    if(xEnd > image.width() || yXP > image.height()){
        status = BAR_OFF;
        return(false);
    }


//    QImage icotmp=image.copy(xBegin, yXP-5, xEnd-xBegin,  11);
//    QString fn;
//    QTextStream(&fn)<< barID << ".png";


             // image.setPixel(i, bar[j].getY(), qRgb(128, 128, 128));

    for(int j=xBegin;j<xEnd;j++) {
        if(findPixel(image, j, 2, barcolor, 5)){
//            icotmp.setPixel(j, 0, qRgb(128, 128, 128));
//            icotmp.setPixel(j, 6, qRgb(0, 255, 0));
//            icotmp.setPixel(j, 10, qRgb(128, 128, 128));
            counter++;
            validator++;
            max = (counter > max)? counter: max;
            position = (counter == max)? j:position;
        } else if(findPixel(image, j, 2, barbkclr, 5)){
//            icotmp.setPixel(j, 0, qRgb(255, 255, 255));
//            icotmp.setPixel(j, 6, qRgb(255, 255, 0));
//            icotmp.setPixel(j, 10, qRgb(255, 255, 255));
            counter--;
            validator++;
            max = (counter > max)? counter: max;
            position = (counter == max)? j:position;
        }
    }
    position = (position <xBegin )? xBegin:position;
    position = (position >xEnd )? xEnd:position;

//    icotmp.setPixel(position, 0, qRgb(0, 0, 255));
//    icotmp.setPixel(position, 10, qRgb(0, 0, 255));

//    icotmp.save(fn);

    XP = ((position- xBegin)*100)/barsize;

    if(validator < barsize/20){
        //qDebug("XPBar::checkXPBar validator %d is under limit. Status^ %d", barID, status == BAR_ON);
         if(barID == idMobHP || barID == idMobMP ){
             XP = XP_ERR; //0;
         } else {
             XP = XP_ERR;
         }
    }
    status = BAR_ON;
    return(true);
}
*/

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

    XP = checkXPBarPartial(image, xBegin, xEnd);
    status = BAR_ON;
    return(true);
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
