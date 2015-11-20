#ifndef DONGLE_H
#define DONGLE_H

#include <windows.h>
#include <hidapi.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <QThread>
#include <QTime>

#include "l2window.h"

#define CMD_WRITE_CONFIG        0
#define CMD_READ_CONFIG         1
#define CMD_ADD_NODE            2
#define CMD_DELETE_NODE         3
#define CMD_DELETE_ALL          4
#define CMD_SET_MODIFIER        5
#define CMD_SET_HPCPMP          6
#define CMD_ADD_NODE_CONDITION  7

#define INTERFACE_ID_Keyboard   0     /**< Keyboard interface descriptor ID */
#define INTERFACE_ID_Mouse      1     /**< Mouse interface descriptor ID */
#define INTERFACE_ID_GenericHID 2     /**< GenericHID interface descriptor ID */

#define STATE_DISCONNECTED     -1
#define STATE_OFF               0
#define STATE_ON                1
#define STATE_TRANSMIT_CONF     2
#define STATE_TRANSMIT_KEY      3


#define DO_NORMAL               0
#define DO_WRITEALLTODONGLE     1
#define DO_WRITEKEYTODONGLE     2
#define DO_SETDONGLEON          3
#define DO_SETDONGLEOFF         4

#define HID_REPORT_SIZE         9

#define KEY_DB_SIZE         219




class Dongle : public QObject
{
    Q_OBJECT

public:
    Dongle();

    void doSendKeyToDongle(int condition_index);
    void doSetState(bool stt);
    void doSaveAllToDongle();

    void setActiveL2W(L2Window* l2w);


    static char* KeyMnemonicDB[KEY_DB_SIZE];
    static unsigned char KeyCodesDB[KEY_DB_SIZE];

 public slots:
    void process();

signals:
    void finished();
    void error(QString err);
    void showStatus(int stt, int updatetime);

private:
    hid_device *handle;
    int state;
    int activity;
    L2Window* currentl2w;
    bool key_transfer_state[KEYNUM];

    int connect();
    int disconnect();
    void recieve_status();
    int send_command(int device, int command, unsigned char* cmd_arg);
    unsigned char string2keycode(QString Key);
    void transmitHPCPMP();
    void sendCMD_SET_STATE(bool stt);
    void sendCMD_WRITE_CONFIG();
    void sendCMD_READ_CONFIG();
    void sendCMD_ADD_NODE(QString Key, float PauseTime, float ReleaseTime, float ConditionTime);
    void sendCMD_DELETE_NODE(QString Key);
    void sendCMD_DELETE_ALL();
    void sendCMD_SET_MODIFIER(unsigned char Modifier);
    void sendCMD_ADD_NODE_CONDITION(QString Key, unsigned char Type, unsigned char Min, unsigned char Max);
    unsigned char getXP(int idXP);
    void sendKeyToDongle(int condition_index);


};

#endif // DONGLE_H
