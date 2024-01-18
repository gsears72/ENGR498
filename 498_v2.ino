#include <Wire.h>
#include <SPI.h>
#include<avr/dtostrf.h>

// I2C addresses for the Honeywell pressure sensors
#define I2C_ADDRESS_1 0x28

// SPI pins for the third Honeywell pressure sensor
#define SPI_CS_PIN 19

uint8_t cmd[3] = {0xAA, 0x00, 0x00}; // command to be sent
double outputmax = 15099494; // output at maximum pressure [counts]
double outputmin = 1677722; // output at minimum pressure [counts]
double pmax = 1; // maximum value of pressure range [bar, psi, kPa, etc.]
double pmin = 0; // minimum value of pressure range [bar, psi, kPa, etc.]

void setup() {
  Serial.begin(9600);

  // Initialize I2C communication
  Wire.begin();

  // Initialize SPI communication
  SPI.begin();
  pinMode(SPI_CS_PIN, OUTPUT);
  digitalWrite(SPI_CS_PIN, HIGH); // Deselect the SPI device initially


}

void loop() {
  // Read data from the first I2C pressure sensor
  float pressure1 = readPressureI2C(I2C_ADDRESS_1);
  Serial.print("Pressure 1: ");
  Serial.print(pressure1);
  Serial.println(" psi");

  // Read data from the SPI pressure sensor
  float pressure3 = readPressureSPI();
  Serial.print("Pressure 3: SPI ");
  Serial.print(pressure3);
  Serial.println(" psi");

  delay(1000); // Delay for 1 second
}

float readPressureI2C(int sensorAddress) {
  Wire.beginTransmission(sensorAddress);
  Wire.write (cmd, 3); // Command byte for pressure data
  Wire.endTransmission();

  delay(10); // Wait for the sensor to process the request

  Wire.requestFrom(sensorAddress, 7);

  byte I2C_buffer[7];
  for (int i = 0; i < 7; i++) {
    I2C_buffer[i] = Wire.read();
  }

  double I2C_press_counts = I2C_buffer[3] + I2C_buffer[2] * 256 + I2C_buffer[1] * 65536; // calculate digital pressure counts
  double pressure = ((I2C_press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;

  return pressure;
}

float readPressureSPI() {
  uint8_t SPIdata[7] = {0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // holds output data
  SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0)); //SPI at 200kHz
  digitalWrite(SPI_CS_PIN, LOW); // set SS Low
  SPI.transfer(cmd, 3); // send Read Command
  digitalWrite(SPI_CS_PIN, HIGH); // set SS High
  delay(10); // wait for conversion
  digitalWrite(SPI_CS_PIN, LOW);
  SPI.transfer(SPIdata, 7);
  digitalWrite(SPI_CS_PIN, HIGH); 
  SPI.endTransaction();
  double SPI_press_counts = SPIdata[3] + SPIdata[2] * 256 + SPIdata[1] * 65536; // calculate digital pressure counts
  //calculation of pressure value according to equation 2 of datasheet
  double pressure = ((SPI_press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;
  
  return pressure;
}
