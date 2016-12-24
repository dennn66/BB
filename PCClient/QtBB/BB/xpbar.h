#ifndef XPBAR_H
#define XPBAR_H

#include <QtWinExtras/QtWin>
#include <QImage>
#include <QTextStream>

#define BAR_OFF false
#define BAR_ON true
#define XP_ERR 128

#define idCP 0
#define idHP 1
#define idMP 2
#define idVP 3
#define idMobHP 4
#define idMobMP 5
#define idTargetType 6 //Only for message to dongle
#define BARNUM 6

#define CP_COLOR qRgb(136, 90, 0) //$CP_COLOR = 0x805300,+(133,100,32)
#define BK_CP_COLOR qRgb(52,42,14) //$CP_COLOR = 0x805300,+
#define HP_COLOR qRgb(121, 28, 17) //$HP_COLOR = 0x881D18,+
#define BK_HP_COLOR qRgb(56,33,30) //$HP_COLOR = 0x881D18,+
//#define BK_HP_COLOR qRgb(47,30,27) //$HP_COLOR = 0x881D18,+
#define MP_COLOR qRgb(5, 57, 134) //$MP_COLOR = 0x03327E,+
#define BK_MP_COLOR qRgb(25,38,60) //$MP_COLOR = 0x03327E,+
#define MobHP_COLOR qRgb(111,23,19) //$MobHP_COLOR = 0x881D18,+
#define BK_MobHP_COLOR qRgb(54,30,28) //$MobHP_COLOR = 0x881D18,+
//#define BK_MobHP_COLOR qRgb(48,28,25) //$MobHP_COLOR = 0x881D18,+
#define MobMP_COLOR qRgb(5,63,130) //$PET_MP_COLOR = 0x074C9C,+
#define BK_MobMP_COLOR qRgb(26,41,67) //$PET_MP_COLOR = 0x074C9C,+
//#define BK_MobMP_COLOR qRgb(24,39,59) //$PET_MP_COLOR = 0x074C9C,+
#define VP_COLOR qRgb(65, 90, 24)
#define BK_VP_COLOR qRgb(32, 37, 25)
#define STAR_COLOR qRgb(0xB7, 0x96, 0x05)

class XPBar
{
public:
    int yXP;
    int xBegin;
    int xEnd;

    XPBar();
    bool getStatus();
    int getXP();
    int getY();
    int getBegin();
    int getEnd();
    bool findXPBar(QImage image, RECT targetRect);
    bool checkXPBar(QImage image);
    void setColors(QRgb color, QRgb bk_color);
    void setbarID(int id);
    void setStatus(bool stt);
    static bool CompareColors(QRgb color1, QRgb color2, UINT8 delta);



private:
    bool status;
    int barID;
    int XP;
    QRgb barcolor;
    QRgb barbkclr;

    bool findPixel(QImage image, int pix_num, UINT8 pixel_delta, QRgb color, UINT8 color_delta);

signals:

public slots:
};

#endif // XPBAR_H
