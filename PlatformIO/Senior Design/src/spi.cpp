#include "spi.h"

void setupSPI() {
  pinMode(sckPin, OUTPUT);
  pinMode(mosiPin, OUTPUT);
  pinMode(misoPin, INPUT);
  pinMode(ssPin, OUTPUT);

  digitalWrite(sckPin, LOW);
  digitalWrite(mosiPin, LOW);
  digitalWrite(ssPin, HIGH);
}

byte transfer(byte data) {
  byte receivedData = 0;

  for (int i = 7; i >= 0; --i) {
    digitalWrite(mosiPin, (data >> i) & 0x01);
    digitalWrite(sckPin, HIGH);
    delayMicroseconds(1);
    digitalWrite(sckPin, LOW);
    receivedData |= (digitalRead(misoPin) << i);
  }

  return receivedData;
}

void transferMultibyte(byte* sendData, byte* receivedData, int length) {
  digitalWrite(ssPin, LOW);

  for (int i = 0; i < length; ++i) {
    receivedData[i] = transfer(sendData[i]);
  }

  digitalWrite(ssPin, HIGH);
}
void readSensorData(double* press_counts, double* temp_counts, double* pressure, double* temperature, double* percentage, double outputmin, double outputmax, double pmin, double pmax) {
  uint8_t data[7] = {0xFA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  uint8_t cmd[3] = {0xAA, 0x00, 0x00};
  transferMultibyte(cmd, data, 3);
  delay(10);
  transferMultibyte(data, data, 7);

  *press_counts = data[3] + data[2] * 256 + data[1] * 65536;
  *temp_counts = data[6] + data[5] * 256 + data[4] * 65536;
  *temperature = (*temp_counts * 200.0 / 16777215.0) - 50.0;
  *percentage = (*press_counts / 16777215.0) * 100.0;

  *pressure = ((*press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin + 1535.7;
}
