/*
------------------------------------------------------------
GlassGarden

File : SensorManager.cpp

وظیفه:
خواندن دما و رطوبت از سنسور DHT
و سطح آب از فلوتر سوئیچ (دو سیمه)
و ذخیره در StateManager

Version : 1.3.0
------------------------------------------------------------
*/

#include "SensorManager.h"

#include <DHT.h>

#include "../core/Config.h"
#include "../state/StateManager.h"

//------------------------------------------------------------
// فاصلهٔ زمانی بین دو خواندن (میلی‌ثانیه)
//------------------------------------------------------------

constexpr unsigned long SENSOR_READ_INTERVAL = 2500;

static DHT dht(DHT_PIN, DHT_TYPE);

SensorManager sensors;

//------------------------------------------------------------
// راه‌اندازی
//------------------------------------------------------------

void SensorManager::begin()
{
    dht.begin();

    // فلوتر سوئیچ: پین ۴ با pull-up داخلی
    // وقتی آب هست → کلید بسته → پین LOW
    // وقتی آب نیست → کلید باز → پین HIGH
    pinMode(WATER_PIN, INPUT_PULLUP);

    state.lastValidSensorReadMs = millis();
}

//------------------------------------------------------------
// بروزرسانی
//------------------------------------------------------------

void SensorManager::update()
{
    if (millis() - lastReadTime < SENSOR_READ_INTERVAL)
    {
        return;
    }

    lastReadTime = millis();

    //--------------------------------------------------------
    // سنسور فلوتر (سطح آب) — همیشه بخوان
    //--------------------------------------------------------

    bool waterPresent = (digitalRead(WATER_PIN) == HIGH);  // HIGH = کلید بسته = آب هست

    state.waterLevelPercent = waterPresent ? 100 : 0;

    if (!state.waterEmpty && !waterPresent)
    {
        state.waterEmpty = true;
        Serial.println("[Sensor] Water EMPTY! Float switch open.");
    }
    else if (state.waterEmpty && waterPresent)
    {
        state.waterEmpty = false;
        Serial.println("[Sensor] Water present. Float switch closed.");
    }

    //--------------------------------------------------------
    // سنسور DHT (دما و رطوبت)
    //--------------------------------------------------------

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
        Serial.println("[Sensor] DHT read failed");
        return;
    }

    state.temperature = t;
    state.humidity = h;
    state.lastValidSensorReadMs = millis();
}