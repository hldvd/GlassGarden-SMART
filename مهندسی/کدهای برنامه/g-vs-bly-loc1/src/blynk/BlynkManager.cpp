/*
------------------------------------------------------------
GlassGarden

File : BlynkManager.cpp

وظیفه:
مدیریت ارتباط با Blynk Cloud

Version : 1.0.0
------------------------------------------------------------
*/

#include "../core/Config.h"

#include <WiFi.h>


#define BLYNK_PRINT Serial
#define BLYNK_HEARTBEAT 30   // افزایش فاصلهٔ heartbeat از 10 به 30 ثانیه

#include <BlynkSimpleEsp32.h>

#include "BlynkManager.h"

#include "../core/Datastreams.h"
#include "../state/StateManager.h"

BlynkManager blynk;   // ← این خط جدیده

unsigned long blynkReconnectTimer = 0;
unsigned long blynkSensorSendTimer = 0;

constexpr unsigned long BLYNK_SENSOR_SEND_INTERVAL = 5000;

//------------------------------------------------------------
// راه‌اندازی
//------------------------------------------------------------

void BlynkManager::begin()
{
    Serial.println("[Blynk] Initializing...");

    Blynk.config(BLYNK_AUTH_TOKEN);

    connected = false;
}

//------------------------------------------------------------
// بروزرسانی
//------------------------------------------------------------

void BlynkManager::update()
{
    if (!state.wifiConnected)
    {
        connected = false;
        state.blynkConnected = false;
        return;
    }

    if (!Blynk.connected())
    {
        if (millis() - blynkReconnectTimer >= 5000)
        {
            blynkReconnectTimer = millis();
            Serial.println("[Blynk] Connecting...");
            Blynk.connect(1000);
        }
    }

    if (Blynk.connected())
    {
        Blynk.run();

        if (!connected)
        {
            connected = true;
            state.blynkConnected = true;
            Serial.println("[Blynk] Connected");
            syncState();
        }

        //------------------------------------------------
        // ارسال staggered: هر 800ms یک virtualWrite
        // به جای 6 تا پشت سر هم
        //------------------------------------------------
        static uint8_t sendStep = 0;
        if (millis() - blynkSensorSendTimer >= 800)
        {
            blynkSensorSendTimer = millis();

            switch (sendStep)
            {
                case 0:  Blynk.virtualWrite(VPIN_TEMPERATURE, state.temperature); break;
                case 1:  Blynk.virtualWrite(VPIN_HUMIDITY, state.humidity); break;
                case 2:  Blynk.virtualWrite(VPIN_LIGHT, state.light ? 1 : 0); break;
                case 3:  Blynk.virtualWrite(VPIN_FOGGER, state.fogger ? 1 : 0); break;
                case 4:  Blynk.virtualWrite(VPIN_FAN, state.fan ? 1 : 0); break;
                case 5:  Blynk.virtualWrite(VPIN_PUMP, state.pump ? 1 : 0); break;
                case 6:  Blynk.virtualWrite(VPIN_WATER_LEVEL, state.waterLevelPercent); break;
            }

            sendStep++;
            if (sendStep > 6) sendStep = 0;
        }
    }
    else
    {
        connected = false;
        state.blynkConnected = false;
    }
}

//------------------------------------------------------------
// همگام‌سازی وضعیت
//------------------------------------------------------------

void BlynkManager::syncState()
{
    Blynk.virtualWrite(VPIN_LIGHT, state.light ? 1 : 0);
    Blynk.virtualWrite(VPIN_FOGGER, state.fogger ? 1 : 0);
    Blynk.virtualWrite(VPIN_FAN, state.fan ? 1 : 0);
    Blynk.virtualWrite(VPIN_PUMP, state.pump ? 1 : 0);

    Blynk.virtualWrite(VPIN_AUTO_MODE, state.autoMode ? 1 : 0);

    Blynk.virtualWrite(VPIN_TEMPERATURE, state.temperature);
    Blynk.virtualWrite(VPIN_HUMIDITY, state.humidity);
}

//------------------------------------------------------------
// وضعیت اتصال
//------------------------------------------------------------

bool BlynkManager::isConnected() const
{
    return connected;
}