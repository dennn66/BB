#ifndef DONGLEWORKER_H
#define DONGLEWORKER_H

#include <QObject>
#include "dongle.h"
#include <QFile>
#include <QTextStream>

class DongleWorker : public QObject {
    Q_OBJECT
private:
    Dongle* dongle;
    QThread* dongle_thread;


public:
    DongleWorker();
    ~DongleWorker();

public slots:
    void process(); 	/*  создает и запускает построитель отчетов */
    void stop();    	/*  останавливает построитель отчетов */
    void setGroupState(int i, bool state);
    void doSetState(bool stt);
    void doSetModifier(bool bCtrl, bool bShift);
    void doSetMode(bool bMode);
    void doSendKeyToDongle(int condition_index);
    void doSaveAllToDongle();
    void doJumpToBootloader();
    void setActiveL2W(L2Window* l2w);
    void showStatus(unsigned char d_stt, unsigned char g_stt, int updatetime);

signals:
    void finished(); 	/* сигнал о завершении  работы построителя отчетов */
    void showDongleStatusSig(unsigned char d_stt, unsigned char g_stt, int updatetime);


};

#endif // DONGLEWORKER_H
