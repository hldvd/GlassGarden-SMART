#pragma once

/*
------------------------------------------------------------
GlassGarden

File : LightDevice.h

وظیفه:
مدیریت روشنایی تراریوم

Version : 1.0.0
------------------------------------------------------------
*/

class LightDevice
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