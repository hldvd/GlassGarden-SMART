/*
------------------------------------------------------------
GlassGarden

File : AutomationManager.cpp

وظیفه:
تصمیم‌گیری خودکار برای کنترل تجهیزات
بر اساس دما، رطوبت و زمان‌بندی

همچنین شامل:
- Minimum ON/OFF Time (30s) برای فن، پمپ و مه‌ساز
  جهت جلوگیری از سوییچینگ سریع و آسیب احتمالی به
  موتورها و قطعات سوییچینگ.
- Safe Mode : اگر بیش از SAFE_MODE_DHT_TIMEOUT_MS
  (2 دقیقه) از آخرین خواندن معتبر DHT گذشته باشد،
  فن/پمپ/مه‌ساز خاموش نگه داشته می‌شوند تا تصمیم‌گیری
  بر اساس دادهٔ نامعتبر/قدیمی رخ ندهد. روشنایی از این
  قانون مستثناست. خروج از Safe Mode خودکار و بلافاصله
  با اولین خواندن موفق بعدی DHT انجام می‌شود.

هر دو محدودیت فقط روی تصمیمات خودکار (این کلاس، حالت
Auto) اعمال می‌شوند؛ فرمان‌های دستی/Blynk تحت تأثیر
قرار نمی‌گیرند.

Version : 1.3.0
------------------------------------------------------------
*/

#include "AutomationManager.h"

#include <Arduino.h>
#include <time.h>

#include "../core/Config.h"
#include "../state/StateManager.h"
#include "../devices/DeviceManager.h"

AutomationManager automation;

//------------------------------------------------------------
// حداقل زمان لازم بین دو تغییر وضعیت متوالی برای تجهیزات
// موتوردار/حساس (فن، پمپ، مه‌ساز)
// طبق تصمیم پروژه: 30 ثانیه، فقط برای حالت Auto
//------------------------------------------------------------

static constexpr unsigned long MIN_STATE_DURATION_MS = 30000;

//------------------------------------------------------------
// حداکثر زمان مجاز بدون خواندن معتبر DHT قبل از ورود به
// Safe Mode
// طبق تصمیم پروژه: 2 دقیقه
//------------------------------------------------------------

static constexpr unsigned long SAFE_MODE_DHT_TIMEOUT_MS = 120000;

//------------------------------------------------------------
// بررسی قرارگیری ساعت جاری در یک بازهٔ زمانی
// (پشتیبانی از بازه‌هایی که از نیمه‌شب عبور می‌کنند)
//------------------------------------------------------------

static bool isWithinSchedule(int onHour, int offHour, int currentHour)
{
    if (onHour == offHour)
    {
        return false;
    }

    if (onHour < offHour)
    {
        return (currentHour >= onHour && currentHour < offHour);
    }

    return (currentHour >= onHour || currentHour < offHour);
}

//------------------------------------------------------------
// راه‌اندازی
//------------------------------------------------------------

void AutomationManager::begin()
{
    // در لحظهٔ بوت، به هر تجهیز اجازهٔ اولین تصمیم‌گیری فوری
    // داده می‌شود (بدون انتظار برای MIN_STATE_DURATION_MS)
    fanLastChangeMs    = 0;
    pumpLastChangeMs   = 0;
    foggerLastChangeMs = 0;
}

//------------------------------------------------------------
// بررسی این‌که آیا اجازهٔ تغییر وضعیت وجود دارد یا خیر
//------------------------------------------------------------

bool AutomationManager::canChangeState(unsigned long lastChangeMs) const
{
    // اگر هنوز هیچ تغییری ثبت نشده (lastChangeMs == 0)، اجازه بده
    if (lastChangeMs == 0)
    {
        return true;
    }

    return (millis() - lastChangeMs) >= MIN_STATE_DURATION_MS;
}

//------------------------------------------------------------
// بروزرسانی
//------------------------------------------------------------

void AutomationManager::update()
{
    if (!AUTOMATION_ENABLED)
    {
        return;
    }

    // اگر حالت روی MANUAL باشد، اتوماسیون هیچ تصمیمی نمی‌گیرد
    // (و Safe Mode هم اعمال نمی‌شود؛ کنترل کامل با کاربر است)
    if (!state.autoMode)
    {
        return;
    }

    //----------------------------------------------------
    // تشخیص Safe Mode بر اساس قطعی طولانی DHT
    //----------------------------------------------------

    unsigned long sinceLastValidRead = millis() - state.lastValidSensorReadMs;

    state.safeMode = (sinceLastValidRead >= SAFE_MODE_DHT_TIMEOUT_MS);

    //----------------------------------------------------
    // Water Empty Protection : فوراً پمپ و مه‌ساز خاموش
    //----------------------------------------------------

    if (state.waterEmpty)
    {
        if (state.fogger)
        {
            devices.foggerOff();
            foggerLastChangeMs = millis();
            Serial.println("[Automation] Fogger forced OFF: water empty");
        }
        if (state.pump)
        {
            devices.pumpOff();
            pumpLastChangeMs = millis();
            Serial.println("[Automation] Pump forced OFF: water empty");
        }
    }

    //----------------------------------------------------
    // بررسی معتبر بودن ساعت (بعد از همگام‌سازی NTP)
    //----------------------------------------------------

    time_t now = time(nullptr);

    bool timeValid = (now > 1700000000);

    int currentHour = -1;

    if (timeValid)
    {
        struct tm timeInfo;

        localtime_r(&now, &timeInfo);

        currentHour = timeInfo.tm_hour;
    }

    //----------------------------------------------------
    // Safe Mode فعال: فن/پمپ/مه‌ساز خاموش نگه داشته
    // می‌شوند و منطق دما/رطوبت/زمان‌بندی برای آن‌ها
    // نادیده گرفته می‌شود. روشنایی مستثناست.
    //----------------------------------------------------

    if (state.safeMode)
    {
        if (state.fan)
        {
            devices.fanOff();
            fanLastChangeMs = millis();
        }

        if (state.pump)
        {
            devices.pumpOff();
            pumpLastChangeMs = millis();
        }

        if (state.fogger)
        {
            devices.foggerOff();
            foggerLastChangeMs = millis();
        }
    }
    else
    {
        //----------------------------------------------------
        // فن : دما + رطوبت + زمان‌بندی
        // + محدودیت Minimum ON/OFF Time (30s)
        //----------------------------------------------------

        bool fanScheduleOn = timeValid && isWithinSchedule(FAN_ON_HOUR, FAN_OFF_HOUR, currentHour);

        bool fanOnCondition = (state.temperature >= FAN_TEMP_ON)
                            || (state.humidity >= FAN_HUMIDITY_ON)
                            || fanScheduleOn;

        bool fanOffCondition = (state.temperature <= FAN_TEMP_OFF)
                            && (state.humidity <= FAN_HUMIDITY_OFF)
                            && !fanScheduleOn;

        if (fanOnCondition && !state.fan && canChangeState(fanLastChangeMs))
        {
            devices.fanOn();
            fanLastChangeMs = millis();
        }
        else if (fanOffCondition && state.fan && canChangeState(fanLastChangeMs))
        {
            devices.fanOff();
            fanLastChangeMs = millis();
        }

        //----------------------------------------------------
        // مه‌ساز : رطوبت + زمان‌بندی
        // + محدودیت Minimum ON/OFF Time (30s)
        // فقط اگر آب موجود باشد
        //----------------------------------------------------

        if (!state.waterEmpty)
        {
            bool foggerScheduleOn = timeValid && isWithinSchedule(FOGGER_ON_HOUR, FOGGER_OFF_HOUR, currentHour);

            bool foggerOnCondition  = (state.humidity <= FOGGER_HUMIDITY_ON) || foggerScheduleOn;
            bool foggerOffCondition = (state.humidity >= FOGGER_HUMIDITY_OFF) && !foggerScheduleOn;

            if (foggerOnCondition && !state.fogger && canChangeState(foggerLastChangeMs))
            {
                devices.foggerOn();
                foggerLastChangeMs = millis();
            }
            else if (foggerOffCondition && state.fogger && canChangeState(foggerLastChangeMs))
            {
                devices.foggerOff();
                foggerLastChangeMs = millis();
            }
        }

        //----------------------------------------------------
        // پمپ : فقط زمان‌بندی
        // + محدودیت Minimum ON/OFF Time (30s)
        // فقط اگر آب موجود باشد
        //----------------------------------------------------

        if (!state.waterEmpty && timeValid)
        {
            bool pumpScheduleOn = isWithinSchedule(PUMP_ON_HOUR, PUMP_OFF_HOUR, currentHour);

            if (pumpScheduleOn && !state.pump && canChangeState(pumpLastChangeMs))
            {
                devices.pumpOn();
                pumpLastChangeMs = millis();
            }
            else if (!pumpScheduleOn && state.pump && canChangeState(pumpLastChangeMs))
            {
                devices.pumpOff();
                pumpLastChangeMs = millis();
            }
        }
    }

    //----------------------------------------------------
    // روشنایی : فقط زمان‌بندی
    // (مستثنا از Safe Mode و از Minimum ON/OFF Time،
    //  طبق تصمیمات پروژه)
    //----------------------------------------------------

    if (timeValid)
    {
        if (isWithinSchedule(LIGHT_ON_HOUR, LIGHT_OFF_HOUR, currentHour))
        {
            if (!state.light)
            {
                devices.lightOn();
            }
        }
        else
        {
            if (state.light)
            {
                devices.lightOff();
            }
        }
    }
}