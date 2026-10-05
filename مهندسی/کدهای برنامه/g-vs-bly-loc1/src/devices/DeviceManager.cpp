/*
------------------------------------------------------------
GlassGarden

File : DeviceManager.cpp

وظیفه:
مدیریت تمام تجهیزات پروژه

تمام درخواست‌های کنترل تجهیزات ابتدا
به این کلاس وارد می‌شوند و سپس به
کلاس مربوط به هر تجهیز ارسال می‌شوند.

Version : 1.2.0
------------------------------------------------------------
*/

#include "DeviceManager.h"
#include "../state/StateManager.h"
#include <Arduino.h>

DeviceManager devices;

//------------------------------------------------------------
// راه‌اندازی تجهیزات
//------------------------------------------------------------

void DeviceManager::begin()
{
    lightDevice.begin();
    fanDevice.begin();
    foggerDevice.begin();
    pumpDevice.begin();
}

//------------------------------------------------------------
// بروزرسانی تجهیزات
//------------------------------------------------------------

void DeviceManager::update()
{
    // Water Empty Protection: در هر حالت (AUTO/MANUAL)
    // اگر آب خالی شد و پمپ/مه‌ساز روشن هستند، فوراً خاموش شوند
    if (state.waterEmpty)
    {
        if (state.fogger)
        {
            foggerOff();
            Serial.println("[DeviceManager] Fogger auto-OFF: water empty");
        }
        if (state.pump)
        {
            pumpOff();
            Serial.println("[DeviceManager] Pump auto-OFF: water empty");
        }
    }
}

//------------------------------------------------------------
// Light
//------------------------------------------------------------

void DeviceManager::lightOn()
{
    lightDevice.on();
    state.light = true;
}

void DeviceManager::lightOff()
{
    lightDevice.off();
    state.light = false;
}

void DeviceManager::lightToggle()
{
    if (state.light)
        lightOff();
    else
        lightOn();
}

bool DeviceManager::lightState() const
{
    return state.light;
}

//------------------------------------------------------------
// Fan
//------------------------------------------------------------

void DeviceManager::fanOn()
{
    fanDevice.on();
    state.fan = true;
}

void DeviceManager::fanOff()
{
    fanDevice.off();
    state.fan = false;
}

void DeviceManager::fanToggle()
{
    if (state.fan)
        fanOff();
    else
        fanOn();
}

bool DeviceManager::fanState() const
{
    return state.fan;
}

//------------------------------------------------------------
// Fogger
//------------------------------------------------------------

void DeviceManager::foggerOn()
{
    if (state.waterEmpty)
    {
        Serial.println("[DeviceManager] Fogger ON blocked: water empty!");
        return;
    }
    foggerDevice.on();
    state.fogger = true;
}

void DeviceManager::foggerOff()
{
    foggerDevice.off();
    state.fogger = false;
}

void DeviceManager::foggerToggle()
{
    if (state.fogger)
        foggerOff();
    else
        foggerOn();
}

bool DeviceManager::foggerState() const
{
    return state.fogger;
}

//------------------------------------------------------------
// Pump
//------------------------------------------------------------

void DeviceManager::pumpOn()
{
    if (state.waterEmpty)
    {
        Serial.println("[DeviceManager] Pump ON blocked: water empty!");
        return;
    }
    pumpDevice.on();
    state.pump = true;
}

void DeviceManager::pumpOff()
{
    pumpDevice.off();
    state.pump = false;
}

void DeviceManager::pumpToggle()
{
    if (state.pump)
        pumpOff();
    else
        pumpOn();
}

bool DeviceManager::pumpState() const
{
    return state.pump;
}

//------------------------------------------------------------
// خاموش کردن همه تجهیزات
//------------------------------------------------------------

void DeviceManager::allOff()
{
    lightOff();
    fanOff();
    foggerOff();
    pumpOff();
}