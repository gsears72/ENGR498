#ifndef SPI_H
#define SPI_H

#include <Arduino.h>

// Define your pin configurations here
const int sckPin = 30;
const int mosiPin = 29;
const int misoPin = 28;
const int ssPin = 19;

extern void setupSPI();
extern byte transfer(byte data);
extern void transferMultibyte(byte* sendData, byte* receivedData, int length);
extern void readSensorData(double* press_counts, double* temp_counts, double* pressure, double* temperature, double* percentage, double outputmin, double outputmax, double pmin, double pmax);

#endif // SPI_H
