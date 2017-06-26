#include "dongleworker.h"

DongleWorker::DongleWorker()
{
    dongle = NULL;
    dongle_thread = new QThread;
    dongle = new Dongle();
    dongle->moveToThread(dongle_thread);

    connect(dongle_thread, SIGNAL(started()), dongle, SLOT(process()));
    connect(dongle, SIGNAL(finished()), dongle_thread, SLOT(quit()));
    connect(dongle, SIGNAL(finished()), dongle, SLOT(deleteLater()));
    connect(dongle_thread, SIGNAL(finished()), dongle_thread, SLOT(deleteLater()));

    connect(dongle, SIGNAL(showStatus(unsigned char, unsigned char,int)), this, SLOT(showStatus(unsigned char, unsigned char,int)));

    dongle_thread->start();




}

DongleWorker::~ DongleWorker ()
{
    if (dongle != NULL) {
        delete dongle;
    }
}

void DongleWorker::process()
{
    //dongle = new Dongle();
    //dongle->process();
    //emit finished();
    return ;
}

void DongleWorker::stop() {
    if(dongle != NULL) {
        //dongle->stop();
    }
    return ;
}

void DongleWorker::setGroupState(int i,  bool state){
    qDebug("DongleWorker::setGroupState(int i,  bool state): n %d st %d", i, state);
    unsigned char current_state;
    if(i < 8){
        current_state = dongle->getGroupState();
        current_state = (state)? (current_state|(1 << i)): (current_state&(~(1 << i)));
        dongle->setGroupState(current_state);
    }
}


void  DongleWorker::doSetState(bool stt){
    qDebug("DongleWorker::doSetState(bool stt): %d", stt);
    unsigned char state = (stt)?(1 << DEVICE_STATUS):0;
    dongle->setDeviceState((dongle->getDeviceState() & (~(1 << DEVICE_STATUS))) | state);
}

void DongleWorker::doSetModifier(bool bCtrl, bool bShift){
    qDebug("DongleWorker::doSetModifier(bool bCtrl, bool bShift): %d %d", bCtrl, bShift);
    unsigned char state = (bCtrl)?(1 << DEVICE_CTRL):0;
    state = (bShift)?(state | (1 << DEVICE_SHIFT)):state;
    dongle->setDeviceState((dongle->getDeviceState() & (~((1 << DEVICE_CTRL) | (1 << DEVICE_SHIFT)))) | state);
}

void DongleWorker::doSetMode(bool bMode){
    qDebug("DongleWorker::doSetMode(bool bMode): %d", bMode);
    unsigned char state = (bMode)?(1 << DEVICE_MODE):0;
    dongle->setDeviceState((dongle->getDeviceState() & (~(1 << DEVICE_MODE))) | state);
}

void DongleWorker::doSendKeyToDongle(int condition_index){
    qDebug("DongleWorker::doSendKeyToDongle(int condition_index)");
    dongle->doSendKeyToDongle(condition_index);

}

void DongleWorker::doSaveAllToDongle(){
    dongle->doSaveAllToDongle();
}


void DongleWorker::doJumpToBootloader(){
    dongle->doJumpToBootloader();
}

void DongleWorker::setActiveL2W(L2Window* l2w)
{
    qDebug("DongleWorker::setActiveL2W");
    dongle->setActiveL2W(l2w);
}

void DongleWorker::showStatus(unsigned char d_stt, unsigned char g_stt,int updatetime)
{
    qDebug("DongleWorker::showDongleStatus(int state, int g_state, int updatetime)");

    emit showDongleStatusSig(d_stt, g_stt,updatetime);
}
