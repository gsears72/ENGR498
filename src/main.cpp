#include "coriolis.h"

// Uncomment test to run specified communications test instead of general data collection
//#define RUN_COMMS_TEST
//#define RUN_SPEED_TEST

// Uncomment to enable debug printing
//#define DEBUG
bool debug = true;

// Change to true to cancel startup calibrate (default is false)
bool calibration = false;

void setup() {
  Serial.begin(115200);

  #ifdef DEBUG
  bool debug = 1;
  #else
  // bool debug = 0;
  #endif
  
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
void loop() {
char printBuffer[200];

  Serial.println("Program Start");

  while(!Serial){delay(1);}
  
  delay(5);
  
  if (debug) {
    
    while(1) {
      
      Serial.print("I2C1:\t");
      readwire(sensor1, false, true, 0);
      
      Serial.print("I2C2:\t");
      readwire(sensor2, false, true, 1);
      
      Serial.print("SPI:\t");
      readspi(false, true, 2);

      vector_output();
      
      delay(100000);

    }
  }
  else {
    while (1) {
      sprintf(printBuffer, "%f", loopcount);
      Serial.print(printBuffer);
      readwire(sensor1, false, false, 0);
      readwire(sensor2, false, false, 1);
      readspi(false, false, 2);
      
      Serial.println();
      // delay(1);
      loopcount++;
    }
  }

}
/*
int main(void) {
  Serial.begin(115200);
  while(!Serial){delay(1);}

  #ifdef DEBUG
  bool debug = 1;
  #else
  bool debug = 0;
  #endif
  
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

  double loopcount = 0;

  #ifdef RUN_COMMS_TEST
  commstest();
  #elif RUN_SPEED_TEST
  speedtest();
  #else
  while(1) {
    getdata_with_print();
    delay(1);
  }
  #endif

  return 0;
}
*/