#include <Arduino.h>
#include <avr/dtostrf.h>

#include "spi.h"
#include "i2c.h"

double press_counts = 0;
double temp_counts = 0;
double pressure = 0;
double temperature = 0;
double outputmax = 15099494;
double outputmin = 1677722;
double pmax = 1245.4445416666501;
double pmin = -1245.4445416666501;
double percentage = 0;

uint8_t spiData[7] = {0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

char printBuffer[200], cBuff[20], percBuff[20], pBuff[20], tBuff[20];

void setup() {
  setupSPI();
  initI2C();
  Serial.begin(9600);
}

void loop() {
  double spi_press_counts = 0, spi_temp_counts = 0, spi_pressure = 0, spi_temperature = 0, spi_percentage = 0;
  double i2c_press_counts = 0, i2c_temp_counts = 0, i2c_pressure = 0, i2c_temperature = 0, i2c_percentage = 0;
  char spi_cBuff[20], spi_percBuff[20], spi_pBuff[20], spi_tBuff[20];
  char i2c_cBuff[20], i2c_percBuff[20], i2c_pBuff[20], i2c_tBuff[20];

  // Read SPI Sensor Data
  readSensorData(&spi_press_counts, &spi_temp_counts, &spi_pressure, &spi_temperature, &spi_percentage, outputmin, outputmax, pmin, pmax);
  dtostrf(spi_press_counts, 4, 1, spi_cBuff);
  dtostrf(spi_percentage, 4, 3, spi_percBuff);
  dtostrf(spi_pressure, 4, 3, spi_pBuff);
  dtostrf(spi_temperature, 4, 3, spi_tBuff);

  // Print SPI Sensor Data
  //Serial.println("\nSPI Sensor Data:");
  //Serial.printf("%x\t%2x %2x %2x\t%s\t%s\t%s\t%s \n", spiData[0], spiData[1], spiData[2], spiData[3], spi_cBuff, spi_percBuff, spi_pBuff, spi_tBuff);

  delay(100);

  // Read I2C Sensor Data
  readSensorDataI2C(0x28, &i2c_press_counts, &i2c_temp_counts, &i2c_pressure, &i2c_temperature, &i2c_percentage, outputmin, outputmax, pmin, pmax);
  dtostrf(i2c_press_counts, 4, 1, i2c_cBuff);
  dtostrf(i2c_percentage, 4, 3, i2c_percBuff);
  dtostrf(i2c_pressure, 4, 3, i2c_pBuff);
  dtostrf(i2c_temperature, 4, 3, i2c_tBuff);

  // Print I2C Sensor Data
  Serial.println("\nI2C Sensor Data:");
  Serial.printf("%x\t%2x %2x %2x\t%s\t%s\t%s\t%s \n", 0xAA, 0x00, 0x00, 0x00, i2c_cBuff, i2c_percBuff, i2c_pBuff, i2c_tBuff);

  delay(100);
}
