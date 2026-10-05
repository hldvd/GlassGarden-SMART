#ifndef HARDWARE_H
#define HARDWARE_H

#include <Arduino.h>

class Hardware
{
public:

    static void begin();

private:

    static void initializePins();

};

#endif