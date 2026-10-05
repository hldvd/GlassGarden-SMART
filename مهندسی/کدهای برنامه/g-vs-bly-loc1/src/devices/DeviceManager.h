#pragma once

/*
------------------------------------------------------------
GlassGarden

File : DeviceManager.h

وظیفه:
مدیریت تمام تجهیزات پروژه

تمام فرمان‌های مربوط به تجهیزات ابتدا
به این کلاس ارسال می‌شوند و سپس به
کلاس مربوط به هر تجهیز منتقل می‌شوند.

Version : 1.1.0
------------------------------------------------------------
*/

#include "LightDevice.h"
#include "FanDevice.h"
#include "FoggerDevice.h"
#include "PumpDevice.h"

class DeviceManager
{
public:

    // راه‌اندازی تجهیزات
    void begin();

    // بروزرسانی تجهیزات
    void update();

    //--------------------------------------------------------
    // Light
    //--------------------------------------------------------

    void lightOn();
    void lightOff();
    void lightToggle();
    bool lightState() const;

    //--------------------------------------------------------
    // Fan
    //--------------------------------------------------------

    void fanOn();
    void fanOff();
    void fanToggle();
    bool fanState() const;

    //--------------------------------------------------------
    // Fogger
    //--------------------------------------------------------

    void foggerOn();
    void foggerOff();
    void foggerToggle();
    bool foggerState() const;

    //--------------------------------------------------------
    // Pump
    //--------------------------------------------------------

    void pumpOn();
    void pumpOff();
    void pumpToggle();
    bool pumpState() const;

    //--------------------------------------------------------
    // خاموش کردن همه تجهیزات
    //--------------------------------------------------------

    void allOff();

private:

    LightDevice   lightDevice;
    FanDevice     fanDevice;
    FoggerDevice  foggerDevice;
    PumpDevice    pumpDevice;

};

//============================================================
// Global Device Manager
//============================================================

extern DeviceManager devices;