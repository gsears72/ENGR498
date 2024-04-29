# serial_config.py

# Define the serial port variable
PORT = "COM8"  # Example port, replace it with your actual port
BAUDRATE = 115200

RESET_SIGNAL = b'\xFF'
COMMS_TEST_SIGNAL = b'\xFA'
SPEED_TEST_SIGNAL = b'\xF0'
READ_SERIAL = b'\xAA'
STOP_SIGNAL = b'\x00'
GET_DATA = b'\xF8'
