#include <Wire.h>
#include <SPI.h>


// I2C multiplexer address
#define MUX_ADDRESS 0x70

// I2C addresses for the Honeywell pressure sensors
#define I2C_ADDRESS_1 0x28
#define I2C_ADDRESS_2 0x29

// SPI pins for the third Honeywell pressure sensor
#define SPI_CS_PIN 10



void setup() {
  Serial.begin(9600);

  // Initialize I2C communication
  Wire.begin();

  // Initialize SPI communication
  SPI.begin();
  pinMode(SPI_CS_PIN, OUTPUT);
  digitalWrite(SPI_CS_PIN, HIGH); // Deselect the SPI device initially

  // Initialize the I2C multiplexer
  Wire.beginTransmission(MUX_ADDRESS);
  Wire.write(0); // Channel 0
  Wire.endTransmission();

}

void loop() {
  // Read data from the first I2C pressure sensor via multiplexer
  float pressure1 = readPressureI2CviaMux(I2C_ADDRESS_1);
  Serial.print("Pressure 1: ");
  Serial.print(pressure1);
  Serial.println(" psi");

  // Read data from the second I2C pressure sensor via multiplexer
  float pressure2 = readPressureI2CviaMux(I2C_ADDRESS_2);
  Serial.print("Pressure 2: ");
  Serial.print(pressure2);
  Serial.println(" psi");

  // Read data from the SPI pressure sensor
  float pressure3 = readPressureSPI();
  Serial.print("Pressure 3: ");
  Serial.print(pressure3);
  Serial.println(" psi");

  delay(1000); // Delay for 1 second
}

float readPressureI2CviaMux(int sensorAddress) {
  Wire.beginTransmission(MUX_ADDRESS);
  Wire.write(1); // Channel 1
  Wire.endTransmission();

  delay(10); // Wait for the multiplexer to switch channels

  Wire.beginTransmission(sensorAddress);
  Wire.write(0x00); // Command byte for pressure data
  Wire.endTransmission();

  delay(10); // Wait for the sensor to process the request

  Wire.requestFrom(sensorAddress, 4);
  while (Wire.available() < 4) {
    // Wait for data to be available
  }

  byte buffer[4];
  for (int i = 0; i < 4; i++) {
    buffer[i] = Wire.read();
  }

  int rawPressure = (buffer[0] << 24) | (buffer[1] << 16) | (buffer[2] << 8) | buffer[3];
  float pressure = rawPressure / 1000.0; // Convert to psi (adjust as needed)

  return pressure;
}

float readPressureSPI() {
  digitalWrite(SPI_CS_PIN, LOW); // Select the SPI device

  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  byte buffer[4];

  // Send dummy byte to read pressure data
  SPI.transfer(0x00);

  // Read 3 bytes of pressure data
  for (int i = 0; i < 3; i++) {
    buffer[i] = SPI.transfer(0x00);
  }

  SPI.endTransaction();

  digitalWrite(SPI_CS_PIN, HIGH); // Deselect the SPI device

  int rawPressure = (buffer[0] << 16) | (buffer[1] << 8) | buffer[2];
  float pressure = rawPressure / 1000.0; // Convert to psi (adjust as needed)

  return pressure;
}
