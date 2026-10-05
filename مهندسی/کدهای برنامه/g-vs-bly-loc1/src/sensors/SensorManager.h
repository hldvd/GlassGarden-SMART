#pragma once

/*
------------------------------------------------------------
GlassGarden

File : SensorManager.h

وظیفه:
خواندن دما و رطوبت از سنسور DHT
و ذخیره در StateManager

Version : 1.0.0
------------------------------------------------------------
*/

#include <Arduino.h>

class SensorManager
{
public:

    // راه‌اندازی
    void begin();

    // بروزرسانی
    void update();

private:

    unsigned long lastReadTime = 0;

};

//============================================================
// Global Sensor Manager
//============================================================

extern SensorManager sensors;