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

    connect(dongle, SIGNAL(showStatus(unsigned char, int)), this, SLOT(showStatus(unsigned char, int)));

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
    unsigned char current_state = dongle->getDeviceState();
    current_state = (state)? (current_state|(1 << (GROUP_0+i))): (current_state&(~(1 << (GROUP_0+i))));
    dongle->setDeviceState(current_state);
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

void DongleWorker::doSendKeyToDongle(int condition_index){
    qDebug("DongleWorker::doSendKeyToDongle(int condition_index)");
    dongle->doSendKeyToDongle(condition_index);

}

void DongleWorker::doSaveAllToDongle(){
    dongle->doSaveAllToDongle();
}


void DongleWorker::setActiveL2W(L2Window* l2w)
{
    qDebug("DongleWorker::setActiveL2W");
    dongle->setActiveL2W(l2w);
}

void DongleWorker::showStatus(unsigned char d_stt, int updatetime)
{
    qDebug("DongleWorker::showDongleStatus(int state, int g_state, int updatetime)");
    emit showDongleStatusSig(d_stt, updatetime);
}
