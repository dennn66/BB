#include "l2parser.h"

L2parser::L2parser(QObject *parent) : QObject(parent)
{
    currentl2w = NULL;

}

void L2parser::process()
{
    qDebug("Start l2 checker");

    while(1){
        QTime updateTime;
        updateTime.start();
        int delta;

        if(currentl2w != NULL) currentl2w->check();

        emit showParserStatus(updateTime.elapsed());

        delta = 100 - updateTime.elapsed();
        delta = (delta > 0)?delta:0;
        #ifdef WIN32
            Sleep(delta);
        #else
            usleep(delta*1000);
        #endif

    }
    emit finished();
}


void L2parser::setActiveL2W(L2Window* l2w)
{
    qDebug("Dongle::setActiveL2W");
    currentl2w = l2w;
}
