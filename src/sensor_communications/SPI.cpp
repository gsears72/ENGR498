#include "sensor_communications.hpp"

SPISettings mySPISettings(SPI_CLK, MSBFIRST, SPI_MODE0);

void sendSPICommand(uint8_t ssPin, const uint8_t* command) {
	uint8_t length = sizeof(command);
	uint8_t commandCopy[length];

	memcpy(commandCopy, command, length);
	SPI.beginTransaction(mySPISettings);
	digitalWrite(ssPin, LOW);
	SPI.transfer(commandCopy, 3);
	digitalWrite(ssPin, HIGH);
	SPI.endTransaction();
}

void readSPIData(uint8_t ssPin, uint8_t* data) {
	SPI.beginTransaction(mySPISettings);
	digitalWrite(ssPin, LOW);
	SPI.transfer(data, 7);
	digitalWrite(ssPin, HIGH);
	SPI.endTransaction();
}