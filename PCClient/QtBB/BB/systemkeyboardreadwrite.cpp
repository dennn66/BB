#include "SystemKeyboardReadWrite.h"

SystemKeyboardReadWrite::SystemKeyboardReadWrite() :
    QObject()
{
    // Assign to null
    keyboardHook = NULL;
}


LRESULT CALLBACK SystemKeyboardReadWrite::keyboardProcedure(int nCode, WPARAM wParam, LPARAM lParam)
{
    qDebug("keyPressed: %d", nCode);


    // Check for a key down press
    if (nCode == HC_ACTION)
    {
        if (wParam == WM_KEYDOWN)
        {
            KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
            emit SystemKeyboardReadWrite::instance()->keyPressed((DWORD) pKeyboard->vkCode);
        }
        else if (wParam == WM_KEYUP)
        {
            KBDLLHOOKSTRUCT *pKeyboard = (KBDLLHOOKSTRUCT*)lParam;
            emit SystemKeyboardReadWrite::instance()->keyReleased((DWORD) pKeyboard->vkCode);
        }
        //reconnect
        instance()->setConnected( false );
        instance()->setConnected( true );
    }

    return false;
}

bool SystemKeyboardReadWrite::connected()
{
    return keyboardHook;
}

bool SystemKeyboardReadWrite::setConnected(bool state)
{
    if(state && keyboardHook == NULL)
    {
        try {
            keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProcedure, GetModuleHandle(NULL), 0);

        } catch(...) {
            ;;
            qDebug("SystemKeyboardReadWrite::SetWindowsHookEx failed\n");

        }

        return keyboardHook;
    }
    else
    {

        try {
            UnhookWindowsHookEx(keyboardHook);
        } catch(...) {
            ;;
            qDebug("SystemKeyboardReadWrite::UnhookWindowsHookEx(keyboardHook) failed\n");

        }
        keyboardHook = NULL;

        return keyboardHook;
    }
}

SystemKeyboardReadWrite* SystemKeyboardReadWrite::instance()
{
    static SystemKeyboardReadWrite* pKeyboardReadWriteInstance = new SystemKeyboardReadWrite();
    qDebug("SystemKeyboardReadWrite:::instance() failed\n");

    return pKeyboardReadWriteInstance;
}
