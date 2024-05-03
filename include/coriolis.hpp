#ifndef CORIOLIS_H
#define CORIOLIS_H

#include "sensor_communications.hpp"
#include "vectormath.hpp"
#include "neopixel.hpp"
#include "serial_config.hpp"

#include <avr/dtostrf.h>
#include <stdio.h>

#include "wiring_private.h"

struct CoriolisProbe {
	Data data;
	Pressures pressures;
	Cvector output;
};

void commsTest(Data* data);
void speedTest();

void getPressures(CoriolisProbe* coriolis);
double extractPressure(uint8_t* data);

void printPressures(Pressures* pressures);
void printData(uint8_t* data);

#endif