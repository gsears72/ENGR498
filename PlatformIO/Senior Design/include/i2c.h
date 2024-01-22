#ifndef I2C_H
#define I2C_H

#include <Arduino.h>

// Define your pin configurations here
const int sdaPin = 26;
const int sclPin = 27;

extern void initI2C();
extern void startI2C();
extern void stopI2C();
extern void writeBitI2C(uint8_t bit);
extern uint8_t readBitI2C();
extern void writeByteI2C(uint8_t data);
extern uint8_t readByteI2C(uint8_t ack);
extern void readSensorDataI2C(int deviceAddress, double* press_counts, double* temp_counts, double* pressure, double* temperature, double* percentage, double outputmin, double outputmax, double pmin, double pmax);

#endif // I2C_H
