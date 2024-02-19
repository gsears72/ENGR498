#ifndef CORIOLIS_H
#define CORIOLIS_H

#include <SPI.h>
#include <Wire.h>
#include <avr/dtostrf.h>
#include "wiring_private.h"
#include <stdio.h>

#include "vectormath.h"

#define delay_time 1000 // reading delay time in ms

// SPIClass Defaults
#define SPI_SERCOM &sercom2
#define MISO 9
#define SCK 8
#define MOSI 10
#define SPI_TX_SETUP SPI_PAD_2_SCK_3
#define SPI_RX_SETUP SERCOM_RX_PAD_0

#define SS PIN_A3

// SPISettings default
#define SPI_CLK 400000
#define SPI_ORDER MSBFIRST
#define SPI_MODE SPI_MODE0

#define sensor1 0x00 // I2C Sensor 1
#define sensor2 0x01 // I2C Sensor 2
#define sensor3 0x04 // Placeholder if using third I2C Sensor

#define outputmax (double)15099494 // output at maximum pressure [counts]
#define outputmin (double)1677722 // output at minimum pressure [counts]
#define pmax5H2O (double)1245.4445416666501 // maximum value of pressure range [bar, psi, kPa, etc.]
#define pmin5H2O (double)-1245.4445416666501 // minimum value of pressure range [bar, psi, kPa, etc.]
#define pmax2H2O (double)498.17781666666 // maximum value of pressure range [bar, psi, kPa, etc.]
#define pmin2H2O (double)-498.17781666666 // minimum value of pressure range [bar, psi, kPa, etc.]

extern double press_counts; // digital pressure reading [counts]
extern double temp_counts; // digital temperature reading [counts]

extern double pressure; // pressure reading [bar, psi, kPa, etc.]
extern double temperature; // temperature reading in deg C

extern double percentage; // holds percentage of full scale data

extern char printBuffer[200], cBuff[20], percBuff[20], pBuff[20], tBuff[20];
extern char fileBuffer[300];

void readspi(bool printres, bool debug = 0);
void readwire(uint8_t sensor, bool printres, bool debug= 0); // takes a target sensor and gathers data from it

void printdata(uint8_t* data, bool debug = 0);

void getdata_with_print();
void getdata();
void commstest();
void speedtest();


#endif