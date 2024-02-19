#include "coriolis.h"

double press_counts = 0; // digital pressure reading [counts]
double temp_counts = 0; // digital temperature reading [counts]

double pressure = 0; // pressure reading [bar, psi, kPa, etc.]
double temperature = 0; // temperature reading in deg C

double percentage = 0; // holds percentage of full scale data

char printBuffer[200], cBuff[20], percBuff[20], pBuff[20], tBuff[20];
char fileBuffer[300];

SPISettings mySPISettings(800000, MSBFIRST, SPI_MODE0);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// GENERAL DATA COLLECTION AND PRINTING FUNCTIONS //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void readwire(uint8_t sensor, bool printres, bool debug) {

  // Set the I2C mux to the correct bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor);
  Wire.endTransmission();

  // Create values to use in communication
  uint8_t id = 0x28; // i2c address
  uint8_t data[7]; // holds output data
  uint8_t cmd[3] = {0xAA, 0x00, 0x00}; // command to be sent

  Wire.beginTransmission(id);
  int stat = Wire.write (cmd, 3); // write command to the sensor
  stat |= Wire.endTransmission(); // end transmission and check for error

  delay(5);

  Wire.requestFrom(id, 7); // read back Sensor data 7 bytes
  int i = 0;
  for (i = 0; i < 7; i++) {
    data [i] = Wire.read();
  }

  if (printres) printdata(data, debug);
}

void readspi(bool printres, bool debug) {
  uint8_t data[7] = {0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // holds output data
  uint8_t cmd[3] = {0xAA, 0x00, 0x00}; // command to be sent

  SPI.beginTransaction(mySPISettings);
  digitalWrite(SS, LOW); // set SS Low
  SPI.transfer(cmd, 3);
  digitalWrite(SS, HIGH); // set SS High

  delay(5); // wait for conversion

  digitalWrite(SS, LOW);
  //mySPI.transfer(data, 7);
  SPI.transfer(data, 7);
  digitalWrite(SS, HIGH);

  //mySPI.endTransaction();
  SPI.endTransaction();

  if (printres) printdata(data, debug);

}

void printdata(uint8_t* data, bool debug) {
  press_counts = data[3] + data[2] * 256 + data[1] * 65536; // calculate digital pressure counts
  temp_counts = data[6] + data[5] * 256 + data[4] * 65536; // calculate digital temperature counts
  temperature = (temp_counts * 200 / 16777215) - 50; // calculate temperature in deg c
  percentage = (press_counts / 16777215) * 100; // calculate pressure as percentage of full scale
  //calculation of pressure value according to equation 2 of datasheet
  pressure = ((press_counts - outputmin) * (pmax5H2O - pmin5H2O)) / (outputmax - outputmin) + pmin5H2O;
  dtostrf(press_counts, 4, 1, cBuff);
  dtostrf(percentage, 4, 3, percBuff);
  dtostrf(pressure, 4, 3, pBuff);
  dtostrf(temperature, 4, 3, tBuff);
  
  if (debug) {
    sprintf(printBuffer, "%x\t%2x\t%2x\t%2x\t%s\t%s\t%s\t%s\n", data[0], data[1], data[2], data[3], cBuff, percBuff, pBuff, tBuff);
  }
  else {
    sprintf(printBuffer, ",%s", pBuff);
  }
  
  Serial.print(printBuffer);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// COMMUNICATIONS TESTS FUNCTIONS ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void commstest() {
  for (int i = 0; i < 10; i++) {
    delay(1);
    sprintf(printBuffer, "%d", i);
    Serial.print(printBuffer);
    readwire(sensor1, true, 1);
    readwire(sensor2, true, 1);
    readspi(true, 1);
    Serial.println();
  }
}

void speedtest() {
  float averageTime = 0.0;
  float totalTime = 0.0;
  float startTime = 0.0;
  float executionTime = 0.0;

  Serial.println("Running communications test with 1000 iterations each averaged.");

  // Set the I2C mux to the second I2C bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor2);
  Wire.endTransmission();

  for(int i = 1; i <= 1000; i++) {
    startTime = micros();
    readwire(sensor1,false);
    executionTime = micros() - startTime;
    totalTime += executionTime;
  }

  averageTime = totalTime / 1000.0;

  sprintf(printBuffer, "Average I2C communication time: %f us", averageTime);
  Serial.println(printBuffer);

  totalTime = 0;

  for(int i = 1; i <= 1000; i++) {
    startTime = micros();
    readspi(false);
    executionTime = micros() - startTime;
    totalTime += executionTime;
  }

  averageTime = totalTime / 1000.0;

  sprintf(printBuffer, "Average SPI communication time: %f us", averageTime);
  Serial.println(printBuffer);

  totalTime = 0;

  for (int i = 1; i < 1000; i++) {
    int y1 = rand() % 1001 - 500;
    int y2 = rand() % 1001 - 500;
    int y3 = rand() % 2491 - 1245;
    startTime = micros();
    calcvector(y1,y2,y3);
    executionTime = micros() - startTime;
    totalTime += executionTime;
  }

  averageTime = totalTime / 1000.0;

  sprintf(printBuffer, "Average calculation time: %f us", averageTime);
  Serial.println(printBuffer);

}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////// OTHER COMMUNICATION FUNCTIONS THAT WILL BE FASTER WHEN COMPLETE ////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void getdata_with_print() {
  // SPI communication information
  uint8_t spidata[7] = {0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // holds output data
  uint8_t spicmd[3] = {0xAA, 0x00, 0x00}; // command to be sent

  // I2C communication information
  uint8_t id = 0x28; // i2c address
  uint8_t i2cdata[7]; // holds output data
  uint8_t i2ccmd[3] = {0xAA, 0x00, 0x00}; // command to be sent

  // Set the I2C mux to the first I2C bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor1);
  Wire.endTransmission();

  // Send command to first I2C sensor to update data
  Wire.beginTransmission(id);
  int stat = Wire.write (i2ccmd, 3); // write command to the sensor
  stat |= Wire.endTransmission(); // end transmission and check for error

  // Set the I2C mux to the second I2C bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor2);
  Wire.endTransmission();

  // Send command to second I2C sensor to update data
  Wire.beginTransmission(id);
  stat = Wire.write (i2ccmd, 3); // write command to the sensor
  stat |= Wire.endTransmission(); // end transmission and check for error

  // Tell SPI sensor to update data
  SPI.beginTransaction(mySPISettings);
  digitalWrite(SS, LOW);
  SPI.transfer(spicmd, 3);
  digitalWrite(SS, HIGH);
  SPI.endTransaction();



  delay(5); // Delay long enough for all sensors to update



  // Set the I2C mux to the first I2C bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor1);
  Wire.endTransmission();

  Wire.requestFrom(id, 7); // read back Sensor data 7 bytes
  int i = 0;
  for (i = 0; i < 7; i++) {
    i2cdata [i] = Wire.read();
  }

  // Calculate I2C sensor 1 pressure value and print to buffer
  press_counts = i2cdata[3] + i2cdata[2] * 256 + i2cdata[1] * 65536; // calculate digital pressure counts
  pressure = ((press_counts - outputmin) * (pmax2H2O - pmin2H2O)) / (outputmax - outputmin) + pmin2H2O;
  sprintf(pBuff, ",%.5f", pressure);
  strcat(printBuffer, pBuff);
  

  // Set the I2C mux to the second I2C bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor2);
  Wire.endTransmission();

  Wire.requestFrom(id, 7); // read back Sensor data 7 bytes
  for (int i = 0; i < 7; i++) {
    i2cdata [i] = Wire.read();
  }

  // Calculate I2C sensor 2 pressure value and print to buffer
  press_counts = i2cdata[3] + i2cdata[2] * 256 + i2cdata[1] * 65536; // calculate digital pressure counts
  pressure = ((press_counts - outputmin) * (pmax2H2O - pmin2H2O)) / (outputmax - outputmin) + pmin2H2O;
  sprintf(pBuff, ",%.5f", pressure);
  strcat(printBuffer, pBuff);


  // Retrieve SPI sensor data
  SPI.beginTransaction(mySPISettings);
  digitalWrite(SS, LOW);
  SPI.transfer(spidata, 7);
  digitalWrite(SS, HIGH);
  SPI.endTransaction();

  // Calculate SPI sensor pressure value and print to buffer
  press_counts = spidata[3] + spidata[2] * 256 + spidata[1] * 65536; // calculate digital pressure counts
  pressure = ((press_counts - outputmin) * (pmax5H2O - pmin5H2O)) / (outputmax - outputmin) + pmin5H2O;
  sprintf(pBuff, ",%.5f", pressure);
  strcat(printBuffer, pBuff);

}

void getdata() {
  // SPI communication information
  uint8_t spidata[7] = {0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // holds output data
  uint8_t spicmd[3] = {0xAA, 0x00, 0x00}; // command to be sent

  // I2C communication information
  uint8_t id = 0x28; // i2c address
  uint8_t i2cdata[7]; // holds output data
  uint8_t i2ccmd[3] = {0xAA, 0x00, 0x00}; // command to be sent

  // Set the I2C mux to the first I2C bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor1);
  Wire.endTransmission();

  // Send command to first I2C sensor to update data
  Wire.beginTransmission(id);
  int stat = Wire.write (i2ccmd, 3); // write command to the sensor
  stat |= Wire.endTransmission(); // end transmission and check for error

  // Set the I2C mux to the second I2C bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor2);
  Wire.endTransmission();

  // Send command to second I2C sensor to update data
  Wire.beginTransmission(id);
  stat = Wire.write (i2ccmd, 3); // write command to the sensor
  stat |= Wire.endTransmission(); // end transmission and check for error

  // Tell SPI sensor to update data
  SPI.beginTransaction(mySPISettings);
  digitalWrite(SS, LOW);
  SPI.transfer(spicmd, 3);
  digitalWrite(SS, HIGH);
  SPI.endTransaction();



  delay(5); // Delay long enough for all sensors to update



  // Set the I2C mux to the first I2C bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor1);
  Wire.endTransmission();

  Wire.requestFrom(id, 7); // read back Sensor data 7 bytes
  for (int i = 0; i < 7; i++) {
    i2cdata [i] = Wire.read();
  }
  

  // Set the I2C mux to the second I2C bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor2);
  Wire.endTransmission();

  Wire.requestFrom(id, 7); // read back Sensor data 7 bytes
  int i = 0;
  for (i = 0; i < 7; i++) {
    i2cdata [i] = Wire.read();
  }


  // Retrieve SPI sensor data
  SPI.beginTransaction(mySPISettings);
  digitalWrite(SS, LOW);
  SPI.transfer(spidata, 7);
  digitalWrite(SS, HIGH);
  SPI.endTransaction();

}
