/*
------------------------------------------------------------
GlassGarden

File : FanDevice.cpp

وظیفه:
مدیریت فن 4 سیمه

GPIO33 → Relay ON/OFF
GPIO4  → PWM Speed Control
GPIO17 → TACH / RPM Feedback

Version : 1.1.0
------------------------------------------------------------
*/

#include "FanDevice.h"
#include "../core/Config.h"
#include "../hardware/Outputs.h"

//------------------------------------------------------------
// Static instance
//------------------------------------------------------------

FanDevice* FanDevice::instance = nullptr;

//------------------------------------------------------------
// راه‌اندازی فن
//------------------------------------------------------------

void FanDevice::begin()
{
    instance = this;

    // تنظیمات PWM سراسری ESP32
    analogWriteFrequency(PWM_FREQUENCY);
    analogWriteResolution(PWM_RESOLUTION);

    // مقدار اولیه PWM = خاموش
    analogWrite(FAN_PWM_PIN, 0);

    Serial.printf(
        "[Fan] PWM GPIO %u | %lu Hz | %u-bit\n",
        FAN_PWM_PIN,
        PWM_FREQUENCY,
        PWM_RESOLUTION
    );

    //--------------------------------------------------------
    // TACH
    //--------------------------------------------------------

    pinMode(FAN_TACH_PIN, INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(FAN_TACH_PIN),
        FanDevice::tachISR,
        FALLING
    );

    tachPulses = 0;
    rpm = 0;
    lastRPMUpdateMs = millis();

    //--------------------------------------------------------
    // وضعیت اولیه
    //--------------------------------------------------------

    speedPercent = 100;

    // فن هنگام شروع خاموش باشد
    off();
}

//------------------------------------------------------------
// بروزرسانی RPM
//------------------------------------------------------------

void FanDevice::update()
{
    const uint32_t now = millis();

    if (now - lastRPMUpdateMs >= 1000)
    {
        noInterrupts();

        const uint32_t pulses = tachPulses;
        tachPulses = 0;

        interrupts();

        rpm =
            (pulses * 60UL) /
            TACH_PULSES_PER_REV;

        lastRPMUpdateMs = now;
    }
}

//------------------------------------------------------------
// روشن کردن فن
//------------------------------------------------------------

void FanDevice::on()
{
    Outputs::fan(true);
    state = true;

    setSpeed(speedPercent);
}

//------------------------------------------------------------
// خاموش کردن فن
//------------------------------------------------------------

void FanDevice::off()
{
    Outputs::fan(false);
    state = false;

    analogWrite(FAN_PWM_PIN, 0);
}

//------------------------------------------------------------
// Toggle
//------------------------------------------------------------

void FanDevice::toggle()
{
    if (state)
        off();
    else
        on();
}

//------------------------------------------------------------
// وضعیت فن
//------------------------------------------------------------

bool FanDevice::isOn() const
{
    return state;
}

//------------------------------------------------------------
// تنظیم سرعت فن
// 0 تا 100 درصد
//------------------------------------------------------------

void FanDevice::setSpeed(uint8_t percent)
{
    if (percent > 100)
        percent = 100;

    speedPercent = percent;

    if (!state)
    {
        analogWrite(FAN_PWM_PIN, 0);
        return;
    }

    const uint32_t maxDuty =
        (1UL << PWM_RESOLUTION) - 1;

    const uint32_t duty =
        (static_cast<uint32_t>(percent) * maxDuty) / 100UL;

    analogWrite(FAN_PWM_PIN, duty);
}

//------------------------------------------------------------
// سرعت تنظیم‌شده
//------------------------------------------------------------

uint8_t FanDevice::getSpeed() const
{
    return speedPercent;
}

//------------------------------------------------------------
// RPM
//------------------------------------------------------------

uint32_t FanDevice::getRPM() const
{
    return rpm;
}

//------------------------------------------------------------
// TACH Interrupt
//------------------------------------------------------------

void IRAM_ATTR FanDevice::tachISR()
{
    if (instance != nullptr)
    {
        instance->tachPulses++;
    }
}