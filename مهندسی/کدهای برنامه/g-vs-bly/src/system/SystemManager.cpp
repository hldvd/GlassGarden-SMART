/*
------------------------------------------------------------
GlassGarden

File : SystemManager.cpp

وظیفه:
راه‌اندازی و مدیریت کل سیستم

Version : 1.0.0
------------------------------------------------------------
*/

#include "SystemManager.h"
#include "../hardware/Hardware.h"
#include "../network/NetworkManager.h"
#include "../devices/DeviceManager.h"
#include "../state/StateManager.h"
#include "../blynk/BlynkManager.h"
#include "../sensors/SensorManager.h"
#include "../automation/AutomationManager.h"

static NetworkManager network;
static BlynkManager blynk;

// معرفی تابع برای جلوگیری از حذف فایل توسط کامپایلر
extern void registerBlynkHandlers(); 

void SystemManager::begin()
{
    Hardware::begin();
    state.begin();
    devices.begin();
    sensors.begin();
    automation.begin();
    network.begin();
    blynk.begin();
    
    // فراخوانی تابع برای فعال‌سازی دکمه‌های دستی
    registerBlynkHandlers(); 
}

void SystemManager::update()
{
    network.update();
    devices.update();
    sensors.update();
    blynk.update();          
    automation.update();     
    state.update();
}