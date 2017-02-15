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

        if(currentl2w != NULL) {
            try {
                currentl2w->check();
                emit isL2Active( currentl2w->isActiveWindow, currentl2w->windowtopright.rx(), currentl2w->windowtopright.ry());
                emit showParserStatus(updateTime.elapsed(), currentl2w->getMainLeftStatus(),  currentl2w->getMainRightStatus(), currentl2w->getMobLeftStatus(), currentl2w->getMobRightStatus(),currentl2w->getToolbarStatus(),  currentl2w->getPetStatus(), currentl2w->getTargetType());
            } catch(...) {
                qDebug("Check failed");
                emit isL2Active( false, 0, 0);
            }

        }
        delta = 50 - updateTime.elapsed();
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
    qDebug("L2parser::setActiveL2W");
    currentl2w = l2w;
}

void L2parser::doActivateL2()
{
    qDebug("L2parser::doActivateL2W()");
    if(currentl2w != NULL)    SetActiveWindow(currentl2w->getHWND());
}

void L2parser::setGroupState(int gr,  bool state){
    if(gr < GROUPSNUM) currentl2w->setGroupState(gr, state);
}
