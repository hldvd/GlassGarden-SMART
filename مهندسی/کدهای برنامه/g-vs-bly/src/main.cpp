/*
------------------------------------------------------------
GlassGarden

File : main.cpp

وظیفه:
این فایل نقطه ورود برنامه است.

فقط مسئول:
- راه‌اندازی اولیه سیستم
- اجرای مدیر اصلی پروژه (SystemManager)

تمام منطق برنامه در کلاس‌های دیگر پیاده‌سازی می‌شود.

Version : 1.0.0
------------------------------------------------------------
*/

#include <Arduino.h>

#include "core/Config.h"
#include "system/SystemManager.h"
#include "blynk/BlynkHandlersHook.h"

// مدیر اصلی نرم‌افزار
SystemManager systemManager;

void setup()
{
    // راه‌اندازی ارتباط سریال
    Serial.begin(SERIAL_BAUDRATE);

    Serial.println();
    Serial.println("========================");
    Serial.println(PROJECT_NAME);
    Serial.println(PROJECT_VERSION);
    Serial.println("========================");

    // این فراخوانی هیچ کاری انجام نمی‌دهد، فقط باعث می‌شود لینکر
    // فایل BlynkHandlers.cpp را حذف نکند (رفع مشکل عدم‌اجرای فرمان‌های دستی)
    registerBlynkHandlers();

    // راه‌اندازی تمام بخش‌های نرم‌افزار
    systemManager.begin();
}

void loop()
{
    // اجرای مداوم بخش‌های مختلف سیستم
    systemManager.update();
}