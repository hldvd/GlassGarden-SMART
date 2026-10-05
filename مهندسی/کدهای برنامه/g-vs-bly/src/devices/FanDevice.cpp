/*
------------------------------------------------------------
GlassGarden

File : FanDevice.cpp

وظیفه:
کنترل فن

Version : 1.0.0
------------------------------------------------------------
*/

#include "FanDevice.h"
#include "../hardware/Outputs.h"

void FanDevice::begin()
{
    off();
}

void FanDevice::on()
{
    Outputs::fan(true);
    state = true;
}

void FanDevice::off()
{
    Outputs::fan(false);
    state = false;
}

void FanDevice::toggle()
{
    if(state)
        off();
    else
        on();
}

bool FanDevice::isOn() const
{
    return state;
}