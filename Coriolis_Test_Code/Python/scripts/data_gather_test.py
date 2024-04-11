import serial
import serial_config as cfg
import sys
import msvcrt
import os

# Adjust the port and baud rate based on your Arduino setup
ser = serial.Serial(cfg.PORT, cfg.BAUDRATE, timeout=1)

# Reset the arduino count if program is running.
ser.write(cfg.RESET_SIGNAL)

ser.write(cfg.GET_DATA)

line = ser.readline().decode('utf-8')
print(line.strip())
line = ser.readline().decode('utf-8')
print(line.strip())
line = ser.readline().decode('utf-8')
print(line.strip())