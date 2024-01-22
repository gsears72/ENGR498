#include "i2c.h"

void initI2C() {
    pinMode(sdaPin, INPUT_PULLUP);  // Set SDA pin as input with internal pull-up resistor
    pinMode(sclPin, INPUT_PULLUP);  // Set SCL pin as input with internal pull-up resistor
}


void startI2C() {
    digitalWrite(sdaPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(sclPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(sdaPin, LOW);
    delayMicroseconds(5);
    digitalWrite(sclPin, LOW);
    delayMicroseconds(5);
}

void stopI2C() {
    digitalWrite(sdaPin, LOW);
    delayMicroseconds(5);
    digitalWrite(sclPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(sdaPin, HIGH);
    delayMicroseconds(5);
}

void writeBitI2C(uint8_t bit) {
    digitalWrite(sdaPin, bit);
    delayMicroseconds(5);
    digitalWrite(sclPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(sclPin, LOW);
    delayMicroseconds(5);
}

uint8_t readBitI2C() {
    digitalWrite(sclPin, HIGH);
    delayMicroseconds(5);
    uint8_t bit = digitalRead(sdaPin);
    digitalWrite(sclPin, LOW);
    delayMicroseconds(5);
    return bit;
}

void writeByteI2C(uint8_t data) {
    for (int i = 7; i >= 0; i--) {
        writeBitI2C((data >> i) & 0x01);
    }
    digitalWrite(sdaPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(sclPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(sclPin, LOW);
    delayMicroseconds(5);
    digitalWrite(sdaPin, LOW);
    delayMicroseconds(5);
}

uint8_t readByteI2C(uint8_t ack) {
    uint8_t data = 0;
    pinMode(sdaPin, INPUT);

    for (int i = 7; i >= 0; i--) {
        data |= (readBitI2C() << i);
    }

    pinMode(sdaPin, OUTPUT);
    digitalWrite(sdaPin, ack ? HIGH : LOW);
    delayMicroseconds(5);
    digitalWrite(sclPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(sclPin, LOW);
    delayMicroseconds(5);

    pinMode(sdaPin, OUTPUT);

    return data;
}

void readSensorDataI2C(int deviceAddress, double* press_counts, double* temp_counts, double* pressure, double* temperature, double* percentage, double outputmin, double outputmax, double pmin, double pmax) {
    startI2C();

    // Address the sensor for write
    writeByteI2C((deviceAddress << 1) | 0);

    // Send command or register address
    writeByteI2C(0x00);

    // Repeat start condition
    startI2C();

    // Address the sensor for read
    writeByteI2C((deviceAddress << 1) | 1);

    // Read data
    uint8_t receivedData[7];
    for (int i = 0; i < 7; i++) {
        receivedData[i] = readByteI2C(i < 6); // Acknowledge all bytes except the last one
    }

    *press_counts = receivedData[3] + receivedData[2] * 256 + receivedData[1] * 65536;
    *temp_counts = receivedData[6] + receivedData[5] * 256 + receivedData[4] * 65536;
    *temperature = (*temp_counts * 200.0 / 16777215.0) - 50.0;
    *percentage = (*press_counts / 16777215.0) * 100.0;

    *pressure = ((*press_counts - outputmin) * (pmax - pmin)) / (outputmax - outputmin) + pmin;

    stopI2C();
}

