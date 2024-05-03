#include "sensor_communications.hpp"

void setI2CLane(uint8_t lane) {
	Wire.beginTransmission(i2cMuxAddr);
	Wire.write(1 << lane);
	Wire.endTransmission();
}

void sendI2CCommand(uint8_t address, const uint8_t* command) {
	int length = sizeof(command);

	Wire.beginTransmission(address);
	int stat = Wire.write(command, length);
	stat |= Wire.endTransmission();
}

void readI2CData(uint8_t address, uint8_t* data) {
	int length = sizeof(data);

	Wire.requestFrom(address,length);
	for (int i = 0; i < length; i++) {
		data[i] = Wire.read();
	}
}

void sweepI2C() {
	byte error, address; 
	int deviceCount = 0;

	Serial.println("Scanning...");

	for (address = 1; address < 127; address++) {
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		if (error == 0) {
			Serial.print("I2C device found at address 0x");
			if (address < 16)
			Serial.print("0");
			Serial.print(address, HEX);
			Serial.println(" !");
			deviceCount++;
		} else if (error == 4) {
			Serial.print("Unknown error at address 0x");
			if (address < 16)
			Serial.print("0");
			Serial.println(address, HEX);
		}
	}
	if (deviceCount == 0)
		Serial.println("No I2C devices found\n");
}