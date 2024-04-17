#include "coriolis.h"
#include "vectormath.h"
#include <tuple>
#include <fstream>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////// GENERAL DATA COLLECTION AND PRINTING FUNCTIONS //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void readwire(uint8_t sensor, bool printres, bool debug, int sensorNum) {

  // Set the I2C mux to the correct bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor);
  Wire.endTransmission();

  // Create values to use in communication
  uint8_t id = 0x28; // i2c address
  uint8_t data[7]; // holds output data
  uint8_t cmd[3] = {0xAA, 0x00, 0x00}; // command to be sent

  //double press_counts = 0; // digital pressure reading [counts]

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
  else dataformate(data,debug,sensorNum);
}

void readspi(bool printres, bool debug, int sensorNum) {
  uint8_t data[7] = {0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}; // holds output data
  uint8_t cmd[3] = {0xAA, 0x00, 0x00}; // command to be sent

  SPISettings mySPISettings(800000, MSBFIRST, SPI_MODE0);
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
  else dataformate(data,debug,sensorNum);

}

void printdata(uint8_t* data, bool debug) {
  double press_counts = 0; // digital pressure reading [counts]
  double temp_counts = 0; // digital temperature reading [counts]
  double pressure = 0; // pressure reading [bar, psi, kPa, etc.]
  double temperature = 0; // temperature reading in deg C
  double percentage = 0; // holds percentage of full scale data
  char cBuff[20], percBuff[20], pBuff[20], tBuff[20], printBuffer[200];


  
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


Cvector output;

void dataformate(uint8_t* data, bool debug,int sensorNum) {
  double press_counts = 0; // digital pressure reading [counts]
  double temp_counts = 0; // digital temperature reading [counts]
  double pressure = 0; // pressure reading [bar, psi, kPa, etc.]
  double temperature = 0; // temperature reading in deg C
  double percentage = 0; // holds percentage of full scale data
  char cBuff[20], percBuff[20], pBuff[20], tBuff[20], printBuffer[200];  

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
  
  if (sensorNum == 0){
    output.set_y1(pressure);
  }
  else if(sensorNum == 1){
    output.set_y2(pressure);
  }
  else if(sensorNum == 2){
    output.set_y3(pressure);
  }

  if (debug) {
    sprintf(printBuffer, "%x\t%2x\t%2x\t%2x\t%s\t%s\t%s\t%s\n", data[0], data[1], data[2], data[3], cBuff, percBuff, pBuff, tBuff);
    std::ofstream file;
    file.open("vector.csv", std::ios::app);
    file << output.get_y1() << "," << output.get_y2() << "," << output.get_y3() << std::endl;
    file.close();
  }
  else {
    sprintf(printBuffer, ",%s", pBuff);
  }
}


std::tuple<double,double,double> startup_calibration_senor_reading(){
  readwire(sensor1, false, false, 0);
  readwire(sensor2, false, false, 1);
  readspi(false, false, 2);
  double sensor1_pressure, sensor2_pressure,spis_pressure;
  sensor1_pressure = output.get_y1();
  sensor2_pressure= output.get_y2();
  spis_pressure = output.get_y3();
  return std::make_tuple(sensor1_pressure, sensor2_pressure, spis_pressure);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// COMMUNICATIONS TESTS FUNCTIONS ////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

void commstest() {
  char printBuffer[200];

  for (int i = 0; i < 10; i++) {
    sprintf(printBuffer, "%d", i);
    Serial.print(printBuffer);
    readwire(sensor1, true, 1);
    readwire(sensor2, true, 1);
    readspi(true, 1, 2);
    Serial.println();
    delay(1);
  }
}

void speedtest() {
  unsigned long averageTime = 0;
  unsigned long totalTime = 0;
  unsigned long startTime = 0;
  unsigned long executionTime = 0;

  Serial.println("Running communications test with 100 iterations each averaged.");

  // Set the I2C mux to the second I2C bus
  Wire.beginTransmission(0x70);
  Wire.write(1 << sensor2);
  Wire.endTransmission();

  for(int i = 1; i <= 100; i++) {
    startTime = millis();
    readwire(sensor1,false);
    executionTime = millis() - startTime;
    totalTime += executionTime;
  }

  averageTime = totalTime / 100.0;

  char printBuffer[200];

  sprintf(printBuffer, "Average I2C communication time: %lu ms", averageTime);
  Serial.println(printBuffer);

  totalTime = 0;

  for(int i = 1; i <= 100; i++) {
    startTime = millis();
    readspi(false);
    executionTime = millis() - startTime;
    totalTime += executionTime;
  }

  sprintf(printBuffer, "Average SPI communication time: %lu ms", averageTime);
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
  SPISettings mySPISettings(800000, MSBFIRST, SPI_MODE0);
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
  double press_counts;
  double pressure;
  char pBuff[20],printBuffer[200];


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
  SPISettings mySPISettings(800000, MSBFIRST, SPI_MODE0);

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
