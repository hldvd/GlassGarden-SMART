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
#include <WiFiManager.h>
#include <WiFi.h>
#include <time.h>
#include "../core/Config.h"
#include "../state/StateManager.h"

// GPIO0 = دکمه BOOT روی اکثر بردهای ESP32
constexpr uint8_t RESET_PIN = 0;

NetworkManager network;

//------------------------------------------------------------
// راه‌اندازی
//------------------------------------------------------------

void NetworkManager::begin()
{
    pinMode(RESET_PIN, INPUT_PULLUP);

    // اگه کاربر هنگام روشن‌کردن، دکمه BOOT رو نگه داشته
    if (digitalRead(RESET_PIN) == LOW)
    {
        Serial.println("[WiFi] Reset button detected, waiting 3s...");
        delay(3000);

        if (digitalRead(RESET_PIN) == LOW)
        {
            WiFiManager wm;
            wm.resetSettings();
            Serial.println("[WiFi] Settings cleared! Restarting...");
            delay(1000);
            ESP.restart();
        }
    }

    WiFiManager wm;

    // تنظیمات پورتال
    wm.setConfigPortalTimeout(180);          // 3 دقیقه timeout
    wm.setConnectTimeout(10);                // 10 ثانیه برای تلاش اتصال
    wm.setAPStaticIPConfig(
        IPAddress(192, 168, 4, 1),
        IPAddress(192, 168, 4, 1),
        IPAddress(255, 255, 255, 0)
    );

    Serial.println("[WiFi] Starting WiFiManager...");

    bool res = wm.autoConnect("GlassGarden-Setup", "12345678");

    if (!res)
    {
        Serial.println("[WiFi] Failed to connect. Restarting...");
        delay(2000);
        ESP.restart();
    }

    Serial.println("[WiFi] Connected!");
    Serial.print("[WiFi] IP : ");
    Serial.println(WiFi.localIP());

    connected = true;
    state.wifiConnected = true;

    WiFi.setSleep(false);
    configTime(NTP_GMT_OFFSET_SEC, NTP_DAYLIGHT_OFFSET_SEC, NTP_SERVER);
    Serial.println("[Time] NTP sync requested");
}

//------------------------------------------------------------
// بروزرسانی (فقط reconnect)
//------------------------------------------------------------

void NetworkManager::update()
{
    if (WiFi.status() == WL_CONNECTED)
    {
        if (!connected)
        {
            connected = true;
            state.wifiConnected = true;
            Serial.println("[WiFi] Reconnected");
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
        WiFi.reconnect();
    }
}

//------------------------------------------------------------
// وضعیت اتصال
//------------------------------------------------------------

bool NetworkManager::isConnected() const
{
    return connected;
}