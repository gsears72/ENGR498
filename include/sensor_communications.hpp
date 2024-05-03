#ifndef SENSOR_COMMUNICATIONS_H
#define SENSOR_COMMUNICATIONS_H

#include <SPI.h>
#include <Wire.h>

/*
I2C Status Byte Codes:
	0xX1 - Math saturation
	0xX4 - Integrity test fail
	0xX5 - Integrity test fail & math saturation
	0x2X - Device is busy
	0x4X - Device is powered
	0x6X - Device is busy & powered
*/

// SPIClass Defaults
#define SPI_SERCOM		&sercom2
#define MISO			9
#define SCK				8
#define MOSI			10
#define SS				PIN_A3
#define SPI_TX_SETUP	SPI_PAD_2_SCK_3
#define SPI_RX_SETUP	SERCOM_RX_PAD_0

// SPISettings default
#define SPI_CLK		800000
#define SPI_ORDER	MSBFIRST
#define SPI_MODE	SPI_MODE0

// I2C Defaults
#define i2cMuxAddr	0x70 // I2C Address
#define sensor1		0x00 // Mux Port
#define sensor1Addr	0x28 // I2C Address
#define sensor2		0x01 // Mux Port
#define sensor2Addr	0x28 // I2C Address
//#define sensor3		0x04 // Placeholder if using third I2C Sensor

// Pressure Value Constants
#define outputmax	(double)15099494 // output at maximum pressure [counts]
#define outputmin	(double)1677722 // output at minimum pressure [counts]
#define pmax5H2O	(double)1245.4445416666501 // maximum value of pressure range [bar, psi, kPa, etc.]
#define pmin5H2O	(double)-1245.4445416666501 // minimum value of pressure range [bar, psi, kPa, etc.]
#define pmax2H2O	(double)498.17781666666 // maximum value of pressure range [bar, psi, kPa, etc.]
#define pmin2H2O	(double)-498.17781666666 // minimum value of pressure range [bar, psi, kPa, etc.]

const uint8_t ABP2_READ[3] = {0xAA, 0x00, 0x00};

struct Data {
	uint8_t pitchData[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t yawData[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t staticData[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
};

struct Pressures {
	double pitchPressure = 0;
	double yawPressure = 0;
	double staticPressure = 0;
};

// I2C.cpp functions
void setI2CLane(uint8_t lane);
void sendI2CCommand(uint8_t address, const uint8_t* command);
void readI2CData(uint8_t address, uint8_t* data);
void sweepI2C();

// SPI.cpp functions
void sendSPICommand(uint8_t ssPin, const uint8_t* command);
void readSPIData(uint8_t ssPin, uint8_t* data);

//sensor_communications.cpp functions
void getSingleI2CData(uint8_t sensor, uint8_t address, uint8_t* data);
void getSingleSPIData(uint8_t* data);
void getDataSerial(Data* data);
void getDataParallel(Data* data);

#endif