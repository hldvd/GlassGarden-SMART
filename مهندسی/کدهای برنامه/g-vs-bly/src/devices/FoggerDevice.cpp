/*
------------------------------------------------------------
GlassGarden

File : FoggerDevice.cpp

وظیفه:
کنترل مه‌ساز

Version : 1.0.0
------------------------------------------------------------
*/

#include "FoggerDevice.h"
#include "../hardware/Outputs.h"

void FoggerDevice::begin()
{
    off();
}

void FoggerDevice::on()
{
    Outputs::fogger(true);
    state = true;
}

void FoggerDevice::off()
{
    Outputs::fogger(false);
    state = false;
}

void FoggerDevice::toggle()
{
    if(state)
        off();
    else
        on();
}

bool FoggerDevice::isOn() const
{
    return state;
}