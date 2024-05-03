#include "coriolis.hpp"

struct CoriolisProbe coriolis;

void setup() {
	Serial.begin(CORIOLISBAUDRATE);

	turnOnLED();

	pinMode(SS, OUTPUT);
	digitalWrite(SS, HIGH);

	pinMode(PIN_A0, OUTPUT);
	digitalWrite(PIN_A0, HIGH);

	pinPeripheral(PIN_SPI_MISO, PIO_SERCOM);
	pinPeripheral(PIN_SPI_MOSI, PIO_SERCOM);
	pinPeripheral(PIN_SPI_SCK, PIO_SERCOM);

	pinPeripheral(PIN_WIRE_SCL, PIO_SERCOM);
	pinPeripheral(PIN_WIRE_SDA, PIO_SERCOM);

	SPI.begin();
	Wire.begin();

	Wire.beginTransmission(0x70);
	Wire.write(1 << 0);
	Wire.endTransmission();
	
}

void loop() {
	while (!Serial){delay(1);}

	while (1) {
		if (Serial.available()) {
			int incomingByte = Serial.read();

			if (incomingByte == GET_DATA) {
				getDataParallel(&coriolis.data);
			}
			else if (incomingByte == PRINT_DATA) {
				getDataParallel(&coriolis.data);

				printData(coriolis.data.pitchData);
				printData(coriolis.data.yawData);
				printData(coriolis.data.staticData);
			}
			else if (incomingByte == GET_PRESSURES) {
				getDataParallel(&coriolis.data);
				getPressures(&coriolis);
			}
			else if (incomingByte == PRINT_PRESSURES) {
				getDataParallel(&coriolis.data);
				getPressures(&coriolis);
				printPressures(&coriolis.pressures);
			}
			else if (incomingByte == GET_VECTOR) {
				getDataParallel(&coriolis.data);
				getPressures(&coriolis);
				coriolis.output.setYs(&coriolis.pressures.pitchPressure, &coriolis.pressures.yawPressure, &coriolis.pressures.staticPressure);
				coriolis.output.calcvector();
			}
			else if (incomingByte == PRINT_VECTOR) {
				getDataParallel(&coriolis.data);
				getPressures(&coriolis);
				coriolis.output.setYs(&coriolis.pressures.pitchPressure, &coriolis.pressures.yawPressure, &coriolis.pressures.staticPressure);
				coriolis.output.calcvector();
				coriolis.output.printVector();
			}
			else if (incomingByte == RUN_COMMS_TEST) {
				commsTest(&coriolis.data);
			}
			else if (incomingByte == RUN_SPEED_TEST) {
				speedTest();
			}				
			else {
				Serial.println("Invalid Command Received.");
			}
		}
	}

	//getDataParallel(&coriolis.sensorData, &coriolis.pitchPressure,&coriolis.yawPressure, &coriolis.staticPressure);
	//printPressures(&coriolis);

	delay(500);
}