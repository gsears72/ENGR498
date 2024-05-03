#include "sensor_communications.hpp"


void getSingleI2CData(uint8_t sensor, uint8_t address, uint8_t* data) {
	setI2CLane(sensor);
	sendI2CCommand(address, ABP2_READ);
	delay(5);
	readI2CData(address, data);
}

void getSingleSPIData(uint8_t* data) {
	sendSPICommand(SS, ABP2_READ);
	delay(5);
	readSPIData(SS, data);
}

void getDataSerial(Data* data) {
	setI2CLane(sensor1);
	sendI2CCommand(sensor1Addr, ABP2_READ);
	delay(5);
	readI2CData(sensor1Addr, data->pitchData);

	setI2CLane(sensor2);
	sendI2CCommand(sensor2Addr, ABP2_READ);
	delay(5);
	readI2CData(sensor2Addr, data->yawData);
	
	sendSPICommand(SS, ABP2_READ);
	delay(5);
	readSPIData(SS, data->staticData);
}

void getDataParallel(Data* data) {
	setI2CLane(sensor1);
	sendI2CCommand(sensor1Addr, ABP2_READ);

	setI2CLane(sensor2);
	sendI2CCommand(sensor2Addr, ABP2_READ);

	sendSPICommand(SS, ABP2_READ);

	delay(5);
	
	readI2CData(sensor2Addr, data->yawData);

	setI2CLane(sensor1);
	readI2CData(sensor1Addr, data->pitchData);

	readSPIData(SS, data->staticData);	
}

