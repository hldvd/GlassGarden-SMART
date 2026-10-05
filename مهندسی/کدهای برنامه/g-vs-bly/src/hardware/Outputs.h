#ifndef OUTPUTS_H
#define OUTPUTS_H

#include <Arduino.h>

class Outputs
{
public:

    static void begin();

    static void light(bool state);

    static void fogger(bool state);

    static void fan(bool state);

    static void pump(bool state);

    static void allOff();

};

#endif