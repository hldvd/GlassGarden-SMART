/*
------------------------------------------------------------
GlassGarden

File : LightDevice.cpp

وظیفه:
کنترل روشنایی

Version : 1.0.0
------------------------------------------------------------
*/

#include "LightDevice.h"
#include "../hardware/Outputs.h"

void LightDevice::begin()
{
    off();
}

void LightDevice::on()
{
    Outputs::light(true);
    state = true;
}

void LightDevice::off()
{
    Outputs::light(false);
    state = false;
}

void LightDevice::toggle()
{
    if(state)
        off();
    else
        on();
}

bool LightDevice::isOn() const
{
    return state;
}