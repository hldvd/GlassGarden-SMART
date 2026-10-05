#pragma once

/*
------------------------------------------------------------
GlassGarden

File : FoggerDevice.h

وظیفه:
مدیریت مه‌ساز

Version : 1.0.0
------------------------------------------------------------
*/

class FoggerDevice
{
public:

    void begin();

    void on();

    void off();

    void toggle();

    bool isOn() const;

private:

    bool state = false;

};