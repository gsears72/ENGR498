#include "coriolis.hpp"

void commsTest(Data* data) {
	for (int i = 0; i < 10; i++) {
		getSingleI2CData(sensor1, sensor1Addr, data->pitchData);
		printData(data->pitchData);

		getSingleI2CData(sensor2, sensor1Addr, data->yawData);
		printData(data->yawData);

		getSingleSPIData(data->staticData);
		printData(data->staticData);
		
		Serial.println();
	}
	Serial.println(STOP_SIGNAL);
}

void speedTest() {
	float averageTime = 0.0;
	float totalTime = 0.0;
	float startTime = 0.0;
	float executionTime = 0.0;

	char printBuffer[50];

	uint8_t data[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	Serial.println("Running communications test with 1000 iterations each averaged.");

	for (int i = 0; i < 1000; i++) {
		startTime = micros();
		getSingleI2CData(sensor1, sensor1Addr, data);
		executionTime = micros() - startTime;
		totalTime += executionTime;
	}

	averageTime = totalTime / 1000;
	totalTime = 0;
	sprintf(printBuffer, "Average I2C communication time: %f us", averageTime);
	Serial.println(printBuffer);

	for (int i = 0; i < 1000; i++) {
		startTime = micros();
		getSingleSPIData(data);
		executionTime = micros() - startTime;
		totalTime += executionTime;
	}

	averageTime = totalTime / 1000;
	totalTime = 0;
	sprintf(printBuffer, "Average SPI communication time: %f us", averageTime);
	Serial.println(printBuffer);

	Serial.println(STOP_SIGNAL);
}

double extractPressure(uint8_t* data) {
	return (((data[3] + data[2] * 256 + data[1] * 65536) - outputmin) * (pmax5H2O - pmin5H2O)) / (outputmax - outputmin) + pmin5H2O;
}

void getPressures(CoriolisProbe* coriolis) {
	coriolis->pressures.pitchPressure = extractPressure(coriolis->data.pitchData);
	coriolis->pressures.yawPressure = extractPressure(coriolis->data.yawData);
	coriolis->pressures.staticPressure = extractPressure(coriolis->data.staticData);
}

// Prints out the current values stored in each of the 
void printPressures(Pressures* pressures) {
	char printBuffer[60];
	sprintf(printBuffer, "Pitch: %f Pa\tYaw: %f Pa\t Static: %f Pa", pressures->pitchPressure,pressures->yawPressure, pressures->staticPressure);
	Serial.println(printBuffer);
	Serial.flush();
}

// Prints out the raw data as well as interpretations of all of the bytes.
void printData(uint8_t* data) {
	char printBuffer[200], cBuff[20], percBuff[20], pBuff[20], tBuff[20];
	double press_counts = data[3] + data[2] * 256 + data[1] * 65536; // calculate digital pressure counts
	double temp_counts = data[6] + data[5] * 256 + data[4] * 65536; // calculate digital temperature counts
	double temperature = (temp_counts * 200 / 16777215) - 50; // calculate temperature in deg c
	double percentage = (press_counts / 16777215) * 100; // calculate pressure as percentage of full scale
	//calculation of pressure value according to equation 2 of datasheet
	double pressure = ((press_counts - outputmin) * (pmax5H2O - pmin5H2O)) / (outputmax - outputmin) + pmin5H2O;
	dtostrf(press_counts, 4, 1, cBuff);
	dtostrf(percentage, 4, 3, percBuff);
	dtostrf(pressure, 4, 3, pBuff);
	dtostrf(temperature, 4, 3, tBuff);

	sprintf(printBuffer, "%2x\t%2x\t%2x\t%2x\t%s\t%s\t%s\t%s\n", data[0], data[1], data[2], data[3], cBuff, percBuff, pBuff, tBuff);
	
	Serial.print(printBuffer);
	Serial.flush();
}



