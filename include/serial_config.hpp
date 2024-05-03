#ifndef SERIAL_CONFIG_H
#define SERIAL_CONFIG_H

#define CORIOLISBAUDRATE	115200

// Serial Communication Codes (Only insert commands below this line)
#define RESET_SIGNAL		0xFF
#define STOP_SIGNAL			0x00
#define GET_DATA			0xA0
#define PRINT_DATA			0xA1
#define GET_PRESSURES		0xB0
#define PRINT_PRESSURES		0xB1
#define GET_VECTOR			0xC0
#define PRINT_VECTOR		0xC1
#define RUN_COMMS_TEST		0xF0
#define RUN_SPEED_TEST		0xF1

#endif