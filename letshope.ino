#include <Wire.h>
#include <avr/dtostrf.h>

#define SDA_PIN_1 26   // Software I2C SDA pin for device 1
#define SCL_PIN_1 27   // Software I2C SCL pin for device 1
#define SDA_PIN_2 5   // Software I2C SDA pin for device 2
#define SCL_PIN_2 6   // Software I2C SCL pin for device 2

const int sckPin = 30;
const int mosiPin = 29;
const int misoPin = 28;
const int ssPin = 19;

double press_counts = 0;
double temp_counts = 0;
double pressure = 0;
double temperature = 0;
double outputmax = 15099494;
double outputmin = 1677722;
double pmax = 498.17781666666;
double pmin = -498.17781666666;
double percentage = 0;
char printBuffer[200], cBuff[20], percBuff[20], pBuff[20], tBuff[20];

void i2cStart(int sdaPin, int sclPin) {
  pinMode(sdaPin, OUTPUT);
  pinMode(sclPin, OUTPUT);
  digitalWrite(sdaPin, HIGH);
  digitalWrite(sclPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(sdaPin, LOW);
  delayMicroseconds(5);
  digitalWrite(sclPin, LOW);
  delayMicroseconds(5);
}

void i2cStop(int sdaPin, int sclPin) {
  pinMode(sdaPin, OUTPUT);
  digitalWrite(sdaPin, LOW);
  pinMode(sclPin, OUTPUT);
  digitalWrite(sclPin, HIGH);
  delayMicroseconds(5);
  pinMode(sdaPin, INPUT);
  delayMicroseconds(5);
}

void i2cWriteByte(int sdaPin, int sclPin, uint8_t byte) {
  for (int i = 7; i >= 0; i--) {
    pinMode(sdaPin, OUTPUT);
    digitalWrite(sdaPin, (byte & (1 << i)) ? HIGH : LOW);
    delayMicroseconds(2);
    digitalWrite(sclPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(sclPin, LOW);
    delayMicroseconds(2);
  }
  pinMode(sdaPin, INPUT);
  delayMicroseconds(2);
  digitalWrite(sclPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(sclPin, LOW);
}

void i2cReadByte(int sdaPin, int sclPin, bool ack) {

  uint8_t data[7] = {0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  pinMode(sdaPin, INPUT);
  for (int i = 7; i >= 0; i--) {
    delayMicroseconds(2);
    digitalWrite(sclPin, HIGH);
    delayMicroseconds(2);
    data[i] = digitalRead(sdaPin);
    digitalWrite(sclPin, LOW);
    delayMicroseconds(2);
  }
  pinMode(sdaPin, OUTPUT);
  digitalWrite(sdaPin, ack ? LOW : HIGH);  // Send ACK or NACK
  delayMicroseconds(2);
  digitalWrite(sclPin, HIGH);
  delayMicroseconds(2);
  digitalWrite(sclPin, LOW);
  delayMicroseconds(2);
  pinMode(sdaPin, INPUT);

  press_counts = data[3] + data[2] * 256 + data[1] * 65536;
  temp_counts = data[6] + data[5] * 256 + data[4] * 65536;
  temperature = (temp_counts * 200 / 16777215) - 50;
  percentage = (press_counts / 16777215.0) * 100;
  
  pressure = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;

  dtostrf(press_counts, 4, 1, cBuff);
  dtostrf(percentage, 4, 3, percBuff);
  dtostrf(pressure, 4, 3, pBuff);
  dtostrf(temperature, 4, 3, tBuff);

  sprintf(printBuffer, "\nStatus Register, 24-bit Sensor data, Digital Pressure Counts,Percentage of full scale pressure,Pressure Output, Temperature\n");
  Serial.println(printBuffer);

  sprintf(printBuffer, "%x\t%2x %2x %2x\t%s\t%s\t%s\t%s \n", data[0], data[1], data[2], data[3],
          cBuff, percBuff, pBuff, tBuff);
  Serial.print(printBuffer);

}


uint8_t SPI_transfer(uint8_t data) {
  uint8_t receivedData = 0;

  for (int i = 7; i >= 0; --i) {
    // Set MOSI (Most Significant Bit first)
    digitalWrite(mosiPin, (data >> i) & 0x01);

    // Pulse SCK to clock data
    digitalWrite(sckPin, HIGH);
    delayMicroseconds(1); // Adjust this delay based on your requirements
    digitalWrite(sckPin, LOW);

    // Read MISO
    receivedData |= (digitalRead(misoPin) << i);

    // Optional: introduce a delay between bits if needed
    // delayMicroseconds(1);
  }

  return receivedData;
}

void SPI_transferMultibyte(byte* sendData, byte* receivedData, int length) {
  digitalWrite(ssPin, LOW); // Select the device

  for (int i = 0; i < length; ++i) {
    receivedData[i] = SPI_transfer(sendData[i]);
  }

  digitalWrite(ssPin, HIGH); // Deselect the device
}

void SPI_readSensorData() {
  uint8_t data[7] = {0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t cmd[3] = {0xAA, 0x00, 0x00};

  SPI_transferMultibyte(cmd, data, 3); // send Read Command
  delay(10); // wait for conversion
  SPI_transferMultibyte(data, data, 7);

  press_counts = data[3] + data[2] * 256 + data[1] * 65536;
  temp_counts = data[6] + data[5] * 256 + data[4] * 65536;
  temperature = (temp_counts * 200 / 16777215) - 50;
  percentage = (press_counts / 16777215.0) * 100;

  pressure = ((press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;

  dtostrf(press_counts, 4, 1, cBuff);
  dtostrf(percentage, 4, 3, percBuff);
  dtostrf(pressure, 4, 3, pBuff);
  dtostrf(temperature, 4, 3, tBuff);

  sprintf(printBuffer, "\nStatus Register, 24-bit Sensor data, Digital Pressure Counts,Percentage of full scale pressure,Pressure Output, Temperature\n");
  Serial.println(printBuffer);

  sprintf(printBuffer, "%x\t%2x %2x %2x\t%s\t%s\t%s\t%s \n", data[0], data[1], data[2], data[3],
          cBuff, percBuff, pBuff, tBuff);
  Serial.print(printBuffer);
}

void setup() {
  Serial.begin(9600);
  pinMode(SDA_PIN_1, INPUT);
  pinMode(SCL_PIN_1, INPUT);
  pinMode(SDA_PIN_2, INPUT);
  pinMode(SCL_PIN_2, INPUT);
  pinMode(sckPin, OUTPUT);
  pinMode(mosiPin, OUTPUT);
  pinMode(misoPin, INPUT);
  pinMode(ssPin, OUTPUT);
  digitalWrite(sckPin, LOW);  // Initialize SCK low
  digitalWrite(mosiPin, LOW); // Initialize MOSI low
  digitalWrite(ssPin, HIGH); 
}

void loop() {

  // Communicate with device 1
  i2cStart(SDA_PIN_1, SCL_PIN_1);
  i2cWriteByte(SDA_PIN_1, SCL_PIN_1, 0x28 << 1);  // Replace with the actual shared address of device 1
  // ... Perform I2C operations for device 1 ...
  i2cReadByte(SDA_PIN_1, SCL_PIN_1, true);
  i2cStop(SDA_PIN_1, SCL_PIN_1);
  delay(100);

  // Communicate with device 2
  i2cStart(SDA_PIN_2, SCL_PIN_2);
  i2cWriteByte(SDA_PIN_2, SCL_PIN_2, 0x28 << 1);  // Replace with the actual shared address of device 2
  // ... Perform I2C operations for device 2 ...
  i2cReadByte(SDA_PIN_2, SCL_PIN_2, true);
  i2cStop(SDA_PIN_2, SCL_PIN_2);
  delay(1000);
}