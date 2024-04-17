#include <calibration.h>
#include <FlashStorage_SAMD.h>

#define CALIBRATION_FLAG_ADDRESS 0 // Address to store the calibration flag in flash memory

#define CALIBRATION_VALUE_1_ADDRESS (CALIBRATION_FLAG_ADDRESS + sizeof(double)) // Address to store the calibration value in flash memory
#define CALIBRATION_VALUE_2_ADDRESS (CALIBRATION_VALUE_1_ADDRESS + sizeof(double)) // Address to store the calibration value in flash memory
#define CALIBRATION_VALUE_3_ADDRESS (CALIBRATION_VALUE_2_ADDRESS + sizeof(double)) // Address to store the calibration value in flash memory

FlashStorage(CALIBRATION_FLAG,bool);
FlashStorage(CALIBRATION_VALUE_1,double);
FlashStorage(CALIBRATION_VALUE_2,double);
FlashStorage(CALIBRATION_VALUE_3,double);


bool calibrate_value_pull() {
    // Pull the calibration flag from flash memory
    bool calibration = CALIBRATION_FLAG.read();
    if (calibration) {
        Serial.println("Reloading Calibration Values");
        return true;
    }
    else {
        Serial.println("Calibrating");
        return false;
    }
}

void calibrate_value_push(bool calibration) {
    // Push the calibration flag to flash memory
    CALIBRATION_FLAG.write(true);
}

#include <tuple> // Include the necessary header file for using std::tuple

void startup_calibrate(double value1, double value2, double value3) {
    // Push the calibration values to flash memory
    CALIBRATION_VALUE_1.write(value1);
    CALIBRATION_VALUE_2.write(value2);
    CALIBRATION_VALUE_3.write(value3);
}

std::tuple<double,double,double> startup_calibrate_pull() {
    // Pull the calibration values from flash memory
    double value1 = CALIBRATION_VALUE_1.read();
    double value2 = CALIBRATION_VALUE_2.read();
    double value3 = CALIBRATION_VALUE_3.read();
    return std::make_tuple(value1, value2, value3);
}
