#ifndef CORIOLIS_H
#define CORIOLIS_H

#include <SPI.h>
#include <Wire.h>
#include <avr/dtostrf.h>
#include "wiring_private.h"
#include <stdio.h>

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
#define sensor2 0x02 // I2C Sensor 2
#define sensor3 0x04 // Placeholder if using third I2C Sensor

#define outputmax (double)15099494 // output at maximum pressure [counts]
#define outputmin (double)1677722 // output at minimum pressure [counts]
#define pmax5H2O (double)1245.4445416666501 // maximum value of pressure range [bar, psi, kPa, etc.]
#define pmin5H2O (double)-1245.4445416666501 // minimum value of pressure range [bar, psi, kPa, etc.]
#define pmax2H2O (double)498.17781666666 // maximum value of pressure range [bar, psi, kPa, etc.]
#define pmin2H2O (double)-498.17781666666 // minimum value of pressure range [bar, psi, kPa, etc.]


double press_counts = 0; // digital pressure reading [counts]
double temp_counts = 0; // digital temperature reading [counts]

double pressure = 0; // pressure reading [bar, psi, kPa, etc.]
double temperature = 0; // temperature reading in deg C

double percentage = 0; // holds percentage of full scale data

char printBuffer[200], cBuff[20], percBuff[20], pBuff[20], tBuff[20];
char fileBuffer[300];


double i2c1_press_counts = 0; // digital pressure reading [counts]
double i2c2_press_counts = 0; // digital pressure reading [counts]
double spi_press_counts = 0; // digital pressure reading [counts]

double i2c1_temp_counts = 0; // digital temperature reading [counts]
double i2c2_temp_counts = 0; // digital temperature reading [counts]
double spi_temp_counts = 0; // digital temperature reading [counts]

double i2c1_pressure = 0; // pressure reading [bar, psi, kPa, etc.]
double i2c2_pressure = 0; // pressure reading [bar, psi, kPa, etc.]
double spi_pressure = 0; // pressure reading [bar, psi, kPa, etc.]

double i2c1_temperature = 0; // temperature reading in deg C
double i2c2_temperature = 0; // temperature reading in deg C
double spi_temperature = 0; // temperature reading in deg C

double i2c1_percentage = 0; // holds percentage of full scale data
double i2c2_percentage = 0; // holds percentage of full scale data
double spi_percentage = 0; // holds percentage of full scale data

SPISettings mySPISettings(800000, MSBFIRST, SPI_MODE0);

void readspi(bool printres, bool debug = 0,int sensorNum = 2);
void readwire(uint8_t sensor, bool printres, bool debug= 0, int sensorNum); // takes a target sensor and gathers data from it

void printdata(uint8_t* data, bool debug = 0);

void getdata_with_print();
void getdata();
void commstest();
void speedtest();


#endif