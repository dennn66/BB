#ifndef DONGLE_H
#define DONGLE_H

#include <windows.h>
#include <hidapi.h>
#include <stdio.h>
#include <wchar.h>
#include <string.h>
#include <QThread>
#include <QTime>
#include <QMessageBox>
#include <QTextStream>


#include "l2window.h"

#define CMD_WRITE_CONFIG        0
#define CMD_READ_CONFIG         1
#define CMD_ADD_NODE            2
#define CMD_DELETE_NODE         3
#define CMD_DELETE_ALL          4
#define CMD_UNUSED              5
#define CMD_SET_HPCPMP          6
#define CMD_ADD_NODE_CONDITION  7
#define CMD_SET_TARGET_STATE    8
#define CMD_SET_SKILL_STATE    9

//#define INTERFACE_ID_Keyboard   0     /**< Keyboard interface descriptor ID */
//#define INTERFACE_ID_Mouse      1     /**< Mouse interface descriptor ID */
//#define INTERFACE_ID_GenericHID 2     /**< GenericHID interface descriptor ID */


#define SERVICE_CONFIG   0     /**< set Keyboard msg */
#define SERVICE_MOUSE      1     /**< set Mouse msg */
#define SERVICE_DEVICE     2     /**< set Device operating mode */
#define SERVICE_KEYBOARD     3     /**< set Device config */

#define STATE_DISCONNECTED     -1
#define STATE_OFF               0
#define STATE_ON                1
#define STATE_TRANSMIT_CONF     2
#define STATE_TRANSMIT_KEY      3
//#define STATE_ACTIVE            4


//#define DEVICE_STATUS_MASK      0b00000001
//#define DEVICE_MODE_MASK        0b00000010
//#define DEVICE_GROUP_MASK       0b00111100


//#define GROUPS_DISCONNECTED     0xFF

//#define DS_ON    0b00000001
//#define DS_OFF   0b00000000

//#define DM_AUTO    0b00000000
//#define DM_DIRECT  0b00000010


#define DO_NORMAL               0
#define DO_WRITEALLTODONGLE     1
#define DO_WRITEKEYTODONGLE     2
#define DO_SETDEVICESTATE       3

#define HID_REPORT_SIZE         9

#define 	HID_KEYBOARD_MODIFIER_LEFTCTRL   (1 << 0)
#define 	HID_KEYBOARD_MODIFIER_LEFTSHIFT   (1 << 1)



// Set device status
// 0 - Device Status
// 1 - Device Mode
// 2 - Group 0 status
// 3 - Group 1 status
// 4 - Group 2 status
// 5 - Group 3 status
// 6 - Ctrl state
// 7 - Shift state
#define DEVICE_STATUS	0
#define DEVICE_MODE		1
#define GROUP_0			2
#define GROUP_1			3
#define GROUP_2			4
#define GROUP_3			5
#define DEVICE_CTRL		6
#define DEVICE_SHIFT	7
#define DEVICE_MASK      0b11111111
#define GROUPS_MASK ((1<<GROUP_0)|(1<<GROUP_1)|(1<<GROUP_2)|(1<<GROUP_3))


class Dongle : public QObject
{
    Q_OBJECT

public:
    Dongle();

    void doSendKeyToDongle(int condition_index);
    void doSaveAllToDongle();
    void setActiveL2W(L2Window* l2w);
    void setDeviceState(unsigned char state){
        qDebug("Dongle::setDeviceState(unsigned int state): %d", state);
        target_device_state = state;
        activity = DO_SETDEVICESTATE;
    }
    unsigned char getDeviceState(){return current_device_state;}

    void setGroupState(unsigned char state){
        qDebug("Dongle::setDeviceState(unsigned int state): %d", state);
        target_group_state = state;
        activity = DO_SETDEVICESTATE;
    }
    unsigned char getGroupState(){return current_group_state;}


 public slots:
    void process();


signals:
    void finished();
    void error(QString err);
    void showStatus(unsigned char d_stt, int updatetime);

private:
    hid_device *handle;
    int state;
    unsigned char target_group_state;
    unsigned char current_group_state;

    unsigned char target_device_state;
    unsigned char current_device_state;
    int activity;
    L2Window* currentl2w;
    bool key_transfer_state[KEYNUM];
    bool bEnableVersion2;
    void spin();

    int connect();
    int disconnect();
    void recieve_status();
    int send_key(unsigned char , bool Ctrl, bool Shift);
    int set_key_report(unsigned char key_code, bool Ctrl, bool Shift);
    int send_command(int device, int command, unsigned char* cmd_arg);
    void transmitHPCPMP();
    void sendCMD_SET_STATE();
    void sendCMD_WRITE_CONFIG();
    void sendCMD_READ_CONFIG();
    void sendCMD_ADD_NODE(unsigned char Key, float PauseTime, float ReleaseTime, float ConditionTime, unsigned int groups, bool Ctrl, bool Shift);
    void sendCMD_DELETE_NODE(unsigned char Key);
    void sendCMD_DELETE_ALL();
    void sendCMD_ADD_NODE_CONDITION(unsigned char Key, unsigned char Type, unsigned char Min, unsigned char Max);
    void sendCMD_SET_TARGET_STATE();
    void sendCMD_SET_SKILL_STATE();

    unsigned char getXP(int idXP);
    unsigned char getTargetType();
    unsigned char getStarState();
    void sendKeyToDongle(int condition_index);


};

#endif // DONGLE_H
