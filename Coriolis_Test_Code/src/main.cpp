#include "coriolis.h"

// Uncomment test to run specified communications test instead of general data collection
//#define RUN_COMMS_TEST
//#define RUN_SPEED_TEST

#define RESET_SIGNAL 0xFF
#define COMMS_TEST_SIGNAL 0xFA
#define SPEED_TEST_SIGNAL 0xF0
#define READ_SERIAL 0xAA
#define STOP_SIGNAL 0x00

// Uncomment to enable debug printing
//#define DEBUG
bool debug = false;

void setup() {
  Serial.begin(115200);
  
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

double loopcount = 0;
uint8_t serialRead = 0;

void loop() {
  while(!Serial){delay(1);}
  delay(5);
  if (debug) {
    while(1) {
      //Serial.print("I2C1:\t");
      //readwire(sensor1, true, true);
      //Serial.print("I2C2:\t");
      //readwire(sensor2, true, true);
      Serial.print("SPI:\t");
      readspi(true, true);
      delay(100);
    }
  }
  else {
    while (1) {
      if (Serial.available()) {
        int incomingByte = Serial.read();

        if (incomingByte == RESET_SIGNAL) {
          loopcount = 0;
          serialRead = 0;
          return;
        }
        else if (incomingByte == COMMS_TEST_SIGNAL) {
          commstest();
          Serial.println("end of test");
        }
        else if (incomingByte == SPEED_TEST_SIGNAL) {
          speedtest();
          Serial.println("end of test");
        }
        else if (incomingByte == READ_SERIAL) {
          serialRead = 1;
          loopcount = 0;
        }
        else if (incomingByte == STOP_SIGNAL) {
          serialRead = 0;
        }
      }

      if (serialRead) {
        delay(1);
        sprintf(printBuffer, "%.0f", loopcount);
        Serial.print(printBuffer);
        readwire(sensor1, true, debug);
        readwire(sensor2, true, debug);
        readspi(true, debug);
        Serial.println();
        loopcount++;
      }
    }
  }
}
