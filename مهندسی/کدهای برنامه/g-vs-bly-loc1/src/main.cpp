/*
------------------------------------------------------------
GlassGarden

File : main.cpp

وظیفه:
این فایل نقطه ورود برنامه است.

فقط مسئول:
- راه‌اندازی اولیه سیستم
- راه‌اندازی آپدیت بی‌سیم (OTA)
- اجرای مدیر اصلی پروژه (SystemManager)

تمام منطق برنامه در کلاس‌های دیگر پیاده‌سازی می‌شود.

Version : 1.1.0
------------------------------------------------------------
*/

#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoOTA.h>
#include "hardware/Outputs.h"
#include "core/Config.h"
#include "system/SystemManager.h"
#include "blynk/BlynkHandlersHook.h"

// ------------------------------------------------------------
// تنظیمات OTA
// نام دستگاه و رمز OTA در ConfigLocal.h تعریف می‌شوند.
// ConfigLocal.h در Git ثبت نمی‌شود.
// ------------------------------------------------------------

// مدیر اصلی نرم‌افزار
SystemManager systemManager;

// آیا سرویس OTA بعد از اتصال Wi-Fi راه‌اندازی شده است؟
static bool otaStarted = false;

// آیا همین الان آپدیت در حال انجام است؟
// در این حالت منطق اصلی برنامه متوقف می‌شود تا با آپدیت تداخل نکند.
static bool otaInProgress = false;

/*
------------------------------------------------------------
خاموش کردن خروجی‌ها در حالت امن

قبل از شروع آپدیت، تجهیزات پرخطر (پمپ، فوگر، رله‌ها و ...)
باید خاموش شوند، چون در طول آپدیت و ریست بعدی، کد اصلی اجرا نمی‌شود.

مهم: پین‌های واقعی پروژه را اینجا بنویسید.
نام‌های زیر فقط نمونه هستند و باید با نام‌های Config.h
خودتان جایگزین شوند. اگر SystemManager تابعی برای خاموش
کردن همه خروجی‌ها دارد، همان را اینجا صدا بزنید.
------------------------------------------------------------
*/
static void setOutputsSafe()
{
    Outputs::allOff();
}

/*
------------------------------------------------------------
تنظیم و راه‌اندازی OTA

این تابع فقط یک بار و بعد از اتصال Wi-Fi صدا زده می‌شود.
------------------------------------------------------------
*/
static void startOta()
{
    ArduinoOTA.setHostname(OTA_HOSTNAME);
    ArduinoOTA.setPassword(OTA_PASSWORD);

    // شروع آپدیت
    ArduinoOTA.onStart([]() {
        otaInProgress = true;
        setOutputsSafe();
        Serial.println("[OTA] Update started");
    });

    // پایان موفق آپدیت (بعد از آن برد خودکار ریست می‌شود)
    ArduinoOTA.onEnd([]() {
        Serial.println("\n[OTA] Update finished, rebooting...");
    });

    // نمایش درصد پیشرفت
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("[OTA] Progress: %u%%\r", (progress * 100) / total);
    });

    // مدیریت خطا
    ArduinoOTA.onError([](ota_error_t error) {
        // آپدیت شکست خورد؛ فریمور قبلی دست‌نخورده است و برنامه ادامه می‌دهد
        otaInProgress = false;
        Serial.printf("[OTA] Error %u: ", error);
        if (error == OTA_AUTH_ERROR)         Serial.println("Auth failed");
        else if (error == OTA_BEGIN_ERROR)   Serial.println("Begin failed");
        else if (error == OTA_CONNECT_ERROR) Serial.println("Connect failed");
        else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive failed");
        else if (error == OTA_END_ERROR)     Serial.println("End failed");
    });

    ArduinoOTA.begin();
    otaStarted = true;

    Serial.print("[OTA] Ready. IP: ");
    Serial.println(WiFi.localIP());
}

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
    // (اتصال Wi-Fi و Blynk داخل SystemManager انجام می‌شود)
    systemManager.begin();
}

void loop()
{
    // بعد از اتصال Wi-Fi، سرویس OTA فقط یک بار فعال می‌شود.
    // این بخش بلاک نمی‌شود؛ اگر Wi-Fi هنوز وصل نیست، برنامه عادی ادامه می‌دهد.
    if (!otaStarted && WiFi.status() == WL_CONNECTED)
    {
        startOta();
    }

    // پردازش درخواست‌های آپدیت؛ باید در هر چرخه loop صدا زده شود
    if (otaStarted)
    {
        ArduinoOTA.handle();
    }

    // در حین آپدیت، منطق اصلی (سنسورها، کنترل، Blynk) متوقف می‌ماند
    if (otaInProgress)
    {
        return;
    }

    // اجرای مداوم بخش‌های مختلف سیستم
    systemManager.update();
}
