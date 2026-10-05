#pragma once

/*
------------------------------------------------------------
GlassGarden

File : PumpDevice.h

وظیفه:
مدیریت پمپ

Version : 1.0.0
------------------------------------------------------------
*/

class PumpDevice
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