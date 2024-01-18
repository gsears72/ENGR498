#include <Arduino.h>
#include <avr/dtostrf.h>

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
double pmax = 1;
double pmin = 0;
double percentage = 0;

char printBuffer[200], cBuff[20], percBuff[20], pBuff[20], tBuff[20];

byte transfer(byte data) {
  byte receivedData = 0;

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

void transferMultibyte(byte* sendData, byte* receivedData, int length) {
  digitalWrite(ssPin, LOW); // Select the device

  for (int i = 0; i < length; ++i) {
    receivedData[i] = transfer(sendData[i]);
  }

  digitalWrite(ssPin, HIGH); // Deselect the device
}

void readSensorData() {
  uint8_t data[7] = {0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t cmd[3] = {0xAA, 0x00, 0x00};

  transferMultibyte(cmd, data, 3); // send Read Command
  delay(10); // wait for conversion
  transferMultibyte(data, data, 7);

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
  pinMode(sckPin, OUTPUT);
  pinMode(mosiPin, OUTPUT);
  pinMode(misoPin, INPUT);
  pinMode(ssPin, OUTPUT);

  digitalWrite(sckPin, LOW);  // Initialize SCK low
  digitalWrite(mosiPin, LOW); // Initialize MOSI low
  digitalWrite(ssPin, HIGH);   // SS should be initially high
  Serial.begin(9600);
}

void loop() {
  delay(1);
  while (1) {
    readSensorData();
    delay(1000);
  }
}
