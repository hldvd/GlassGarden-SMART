#pragma once

/*
------------------------------------------------------------
GlassGarden

File : BlynkManager.h

وظیفه:
مدیریت ارتباط با Blynk Cloud

Version : 1.0.0
------------------------------------------------------------
*/

#include <Arduino.h>

class BlynkManager
{
public:

    // راه‌اندازی
    void begin();

    // بروزرسانی
    void update();

    // همگام‌سازی وضعیت
    void syncState();

    // وضعیت اتصال
    bool isConnected() const;

private:

    bool connected = false;

};