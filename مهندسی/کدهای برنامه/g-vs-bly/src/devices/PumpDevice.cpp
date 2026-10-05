/*
------------------------------------------------------------
GlassGarden

File : PumpDevice.cpp

وظیفه:
کنترل پمپ

Version : 1.0.0
------------------------------------------------------------
*/

#include "PumpDevice.h"
#include "../hardware/Outputs.h"

void PumpDevice::begin()
{
    off();
}

void PumpDevice::on()
{
    Outputs::pump(true);
    state = true;
}

void PumpDevice::off()
{
    Outputs::pump(false);
    state = false;
}

void PumpDevice::toggle()
{
    if(state)
        off();
    else
        on();
}

bool PumpDevice::isOn() const
{
    return state;
}