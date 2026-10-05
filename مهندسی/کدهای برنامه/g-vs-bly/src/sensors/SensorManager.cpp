/*
------------------------------------------------------------
GlassGarden

File : SensorManager.cpp

وظیفه:
خواندن دما و رطوبت از سنسور DHT
و ذخیره در StateManager

همچنین ثبت زمان آخرین خواندن معتبر
(state.lastValidSensorReadMs) که توسط
AutomationManager برای تشخیص Safe Mode
استفاده می‌شود.

Version : 1.1.0
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

    pinMode(WATER_PIN, INPUT);

    // شمارش قطعی سنسور از لحظهٔ بوت آغاز می‌شود، نه از epoch=0
    // (تا Safe Mode بلافاصله بعد از بوت فعال نشود)
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

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t))
    {
        Serial.println("[Sensor] Read failed");
        return;
    }

    state.temperature = t;
    state.humidity = h;
    state.lastValidSensorReadMs = millis();

    //--------------------------------------------------------
    // سطح آب مخزن (سنسور آنالوگ P100)
    //--------------------------------------------------------

    int raw = analogRead(WATER_PIN);

    int percent = map(raw, WATER_LEVEL_EMPTY, WATER_LEVEL_FULL, 0, 100);
    percent = constrain(percent, 0, 100);

    state.waterLevelPercent = percent;

    // هیسترزیس: وقتی خالی شد، تا رسیدن به EMPTY + حاشیه
    // در حالت "خالی" باقی می‌ماند (از نوسان هشدار جلوگیری می‌کند)
    if (!state.waterEmpty && raw <= WATER_LEVEL_EMPTY)
    {
        state.waterEmpty = true;
    }
    else if (state.waterEmpty && raw >= (WATER_LEVEL_EMPTY + WATER_LEVEL_EMPTY_HYSTERESIS))
    {
        state.waterEmpty = false;
    }
}