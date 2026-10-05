#include "Outputs.h"
#include "../core/Config.h"

static void writeOutput(const char *name, uint8_t pin, bool state)
{
    if (OUTPUT_ACTIVE_HIGH)
    {
        digitalWrite(pin, state ? HIGH : LOW);
    }
    else
    {
        digitalWrite(pin, state ? LOW : HIGH);
    }

    Serial.print("[Output] ");
    Serial.print(name);
    Serial.print(" (GPIO ");
    Serial.print(pin);
    Serial.print(") -> ");
    Serial.println(state ? "ON" : "OFF");
}

void Outputs::begin()
{
    allOff();
}

void Outputs::light(bool state)
{
    writeOutput("Light", LIGHT_PIN, state);
}

void Outputs::fogger(bool state)
{
    writeOutput("Fogger", FOGGER_PIN, state);
}

void Outputs::fan(bool state)
{
    writeOutput("Fan", FAN_PIN, state);
}

void Outputs::pump(bool state)
{
    writeOutput("Pump", PUMP_PIN, state);
}

void Outputs::allOff()
{
    light(false);
    fogger(false);
    fan(false);
    pump(false);
}