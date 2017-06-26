#ifndef SYSTEMMOUSEHOOK_H
#define SYSTEMMOUSEHOOK_H
#pragma once
#include <QObject>
#include <QMainWindow>
#include <Windows.h>
#include <QMessageBox>
#include <QTextStream>

class SystemMouseHook : public QObject
{

Q_OBJECT
public:
    // Returns singleton instance
    static SystemMouseHook * instance();

    // Returns whether the keyboard hook is connected
    bool connected();
    // Connects / Disconnects the keyboard hook
    bool setConnected(bool state);

signals:
    // Broadcasts a key has been pressed
    void keyLPressed(int x, int y);
    // Broadcasts a key has been released
    void keyLReleased(int x, int y);

private:
    // Class constructor
    SystemMouseHook();

    // Keyboard hook
    HHOOK mouseHook;



    // Identifies hook activity
    static LRESULT CALLBACK mouseProcedure(int nCode, WPARAM wParam, LPARAM lParam);

};
#endif // SYSTEMMOUSEHOOK_H
