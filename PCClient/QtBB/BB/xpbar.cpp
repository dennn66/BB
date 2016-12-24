#include "xpbar.h"

XPBar::XPBar()
{
    status = BAR_OFF;
    XP = XP_ERR;
    xBegin = 0;
    xEnd = 0;
    yXP=0;

}
/*
bool XPBar::findXPBar(QImage image,  RECT targetRect){

    RECT rect;
    qDebug("XPBar::findXPBar %d", barID);

    qDebug("%d il: %d ir: %d", barID, targetRect.left, targetRect.right);
    qDebug("%d it: %d ib: %d", barID, targetRect.top, targetRect.bottom);
    qDebug("%d w: %d h: %d", barID, image.width(), image.height());

    rect.left = (targetRect.left <0)?0:targetRect.left;
    rect.top = (targetRect.top <0)?0:targetRect.top;
    rect.right = (targetRect.right>image.width())?image.width():targetRect.right;
    rect.bottom = (targetRect.bottom > image.height())?image.height():targetRect.bottom;

    qDebug("%d l: %d r: %d", barID, rect.left, rect.right);
    qDebug("%d t: %d b: %d", barID, rect.top, rect.bottom);

    if( rect.right <= rect.left || rect.bottom <= rect.top){
        status = BAR_OFF;
        XP = XP_ERR;
        xBegin = 0;
        xEnd = 0;
        yXP=0;
        return status;
    }
    xBegin = rect.left;
    xEnd = rect.right;
    yXP=0;

    int max = 0;


    QImage icotmp=image.copy(rect.left, rect.top, rect.right-rect.left,  rect.bottom-rect.top);
    QString fn;
    QTextStream(&fn)<< barcolor << ".png";
    icotmp.save(fn);
    qDebug(fn.toStdString().c_str());


    for(int j=rect.top;j<rect.bottom;j++) {
        int count = 0;
        for(int i=rect.left;i<rect.right;i++){
             QRgb pix = image.pixel(QPoint(i,j));
//            qDebug("%d ->  : (y:%d x:%d)", barID, j,  i);

            if(CompareColors(pix, barcolor, 5)|| CompareColors(pix, barbkclr, 5)) {
               count++;
            }
        }
        max = (count > max)? count:max;
        yXP = (count == max)?j:yXP;
    }
//    qDebug("->  : (bar:%d yXP:%d barsize: %d)", barID,  yXP, max);

    if(max < 100) {
        xBegin = xEnd = yXP = 0;
        status = BAR_OFF;
        XP = XP_ERR;
        qDebug("%d bar not found", barID);
        return status;
    }

    int counter = 0;
    int position = rect.left;
    max = 0;

    for(int j=rect.left;j<rect.right;j++) {
        QRgb pix = image.pixel(QPoint(j,yXP));
        if(CompareColors(pix, barcolor, 5)){
            counter++;
            max = (counter > max)? counter: max;
            position = (counter == max)? j:position;
//            if(barID == 1) qDebug("%d ->  : (j:%d ++, counter: %d)", barID, j, counter);
        } else if(CompareColors(pix, barbkclr, 5)){
            counter--;
//            if(barID == 1) qDebug("%d ->  : (j:%d --, counter: %d)", barID, j, counter);
            max = (counter > max)? counter: max;
            position = (counter == max)? j:position;
        }
    }
//    qDebug("-position:%d ", position);

    xBegin = xEnd = position;

    for(int i=position;i>=rect.left;i--){
         QRgb pix = image.pixel(QPoint(i,yXP));
        if(CompareColors(pix, barcolor, 5)) {
           xBegin = i;
        }
    }
//    qDebug("-xbegin:%d ", xBegin);

    for(int i=position;i<=rect.right;i++){
         QRgb pix = image.pixel(QPoint(i,yXP));
        if(CompareColors(pix, barbkclr, 5)) {
           xEnd = i;
        }
    }
//    qDebug("-xend:%d ", xEnd);


    if(     (xBegin < xEnd) &&
            (xBegin > 0) &&
            (xEnd < image.width()) &&
            (yXP > 0) &&
            (yXP < image.height())){
        status = BAR_ON;
    } else {
        xBegin = xEnd = yXP = 0;
        status = BAR_OFF;
    }
    XP = XP_ERR;
    qDebug("%d ->  : xBegin:%d xEnd:%d yXP:%d ", barID, xBegin, xEnd, yXP);

    return status;
}
*/

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
    int startx = pix_num-pixel_delta;
    int endx = pix_num+pixel_delta;
    int starty = yXP-pixel_delta;
    int endy=yXP+pixel_delta;
    startx = (startx>0)?startx:0;
    starty = (starty>0)?starty:0;
    //qDebug("startx: %d starty: %d", startx, starty);

    for(int row = starty; row < endy; row++){
        for(int col = startx; col < endx; col++){
            //qDebug("%d -> (col: %d row: %d)", barID, col, row);
            QRgb pix = image.pixel(QPoint(col,row));
            if(CompareColors(pix, color, color_delta)){
                //qDebug("return true");
                return true;
            }
        }
    }
    //qDebug("return false");
    return false;
}

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
