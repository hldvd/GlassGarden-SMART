/*
------------------------------------------------------------
GlassGarden

File : NetworkManager.cpp

وظیفه:
مدیریت ارتباط WiFi

Version : 1.0.0
------------------------------------------------------------
*/

#include "NetworkManager.h"

#include <WiFi.h>
#include <time.h>

#include "../core/Config.h"
#include "../state/StateManager.h"

//------------------------------------------------------------
// راه‌اندازی
//------------------------------------------------------------

void NetworkManager::begin()
{
    Serial.println("[WiFi] Connecting...");

    connect();
}

//------------------------------------------------------------
// اتصال به WiFi
//------------------------------------------------------------

void NetworkManager::connect()
{
    WiFi.mode(WIFI_STA);

    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
}

//------------------------------------------------------------
// بروزرسانی
//------------------------------------------------------------

void NetworkManager::update()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        if (!connected)
        {
            connected = true;

            state.wifiConnected = true;

            Serial.println("[WiFi] Connected");

            Serial.print("[WiFi] IP : ");

            Serial.println(WiFi.localIP());

            // غیرفعال کردن Modem Sleep رادیوی WiFi
            // بدون این خط، ESP32 به‌صورت پیش‌فرض بین بسته‌ها
            // رادیو را خاموش/روشن می‌کند و همین باعث تاخیر
            // تجمعی در دریافت فرمان‌های دستی از Blynk Cloud
            // می‌شود (هرچه فرمان بیشتر، تاخیر بیشتر)
            WiFi.setSleep(false);

            // همگام‌سازی ساعت برای اتوماسیون زمان‌بندی‌شده
            configTime(NTP_GMT_OFFSET_SEC, NTP_DAYLIGHT_OFFSET_SEC, NTP_SERVER);

            Serial.println("[Time] NTP sync requested");
        }

        return;
    }

    state.wifiConnected = false;

    connected = false;

    if (millis() - reconnectTimer > 10000)
    {
        reconnectTimer = millis();

        Serial.println("[WiFi] Reconnecting...");

        WiFi.disconnect();

        connect();
    }
}

//------------------------------------------------------------
// وضعیت اتصال
//------------------------------------------------------------

bool NetworkManager::isConnected() const
{
    return connected;
}