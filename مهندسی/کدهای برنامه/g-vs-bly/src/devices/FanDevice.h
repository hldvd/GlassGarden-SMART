#pragma once

/*
------------------------------------------------------------
GlassGarden

File : FanDevice.h

وظیفه:
مدیریت فن

Version : 1.0.0
------------------------------------------------------------
*/

class FanDevice
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