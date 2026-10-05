#pragma once

/*
------------------------------------------------------------
GlassGarden

File : FanDevice.h

وظیفه:
مدیریت فن 4 سیمه

GPIO33 → Relay ON/OFF
GPIO4  → PWM Speed Control
GPIO17 → TACH / RPM Feedback

Version : 1.1.0
------------------------------------------------------------
*/

#include <Arduino.h>

class FanDevice
{
public:

    void begin();

    void update();

    void on();

    void off();

    void toggle();

    bool isOn() const;

    // کنترل سرعت فن
    void setSpeed(uint8_t percent);

    uint8_t getSpeed() const;

    // سرعت واقعی فن
    uint32_t getRPM() const;

private:

    bool state = false;

    uint8_t speedPercent = 100;

    volatile uint32_t tachPulses = 0;

    uint32_t rpm = 0;

    uint32_t lastRPMUpdateMs = 0;

    static void IRAM_ATTR tachISR();

    static FanDevice* instance;

    static constexpr uint32_t PWM_FREQUENCY = 25000;
    static constexpr uint8_t PWM_RESOLUTION = 8;

    // فعلاً استاندارد رایج فن 4 سیمه:
    // دو پالس TACH برای هر دور
    static constexpr uint16_t TACH_PULSES_PER_REV = 2;
};