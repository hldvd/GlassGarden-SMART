#pragma once

/*
------------------------------------------------------------
GlassGarden

File : NetworkManager.h

وظیفه:
مدیریت ارتباط WiFi

Version : 1.0.0
------------------------------------------------------------
*/

#include <Arduino.h>

class NetworkManager
{
public:

    // راه‌اندازی
    void begin();

    // بروزرسانی
    void update();

    // وضعیت اتصال
    bool isConnected() const;

private:

    bool connected = false;

    unsigned long reconnectTimer = 0;

    void connect();

};