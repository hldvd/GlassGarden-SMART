/*
------------------------------------------------------------
GlassGarden

File : Hardware.cpp

وظیفه:
مقداردهی اولیه سخت‌افزار پروژه

Version : 1.0.0

------------------------------------------------------------
*/

#include "Hardware.h"
#include "../core/Config.h"

void Hardware::begin()
{
    initializePins();
}

void Hardware::initializePins()
{
    pinMode(LIGHT_PIN, OUTPUT);
    pinMode(FOGGER_PIN, OUTPUT);
    pinMode(FAN_PIN, OUTPUT);
    pinMode(PUMP_PIN, OUTPUT);

    if (OUTPUT_ACTIVE_HIGH)
    {
        digitalWrite(LIGHT_PIN, LOW);
        digitalWrite(FOGGER_PIN, LOW);
        digitalWrite(FAN_PIN, LOW);
        digitalWrite(PUMP_PIN, LOW);
    }
    else
    {
        digitalWrite(LIGHT_PIN, HIGH);
        digitalWrite(FOGGER_PIN, HIGH);
        digitalWrite(FAN_PIN, HIGH);
        digitalWrite(PUMP_PIN, HIGH);
    }
}