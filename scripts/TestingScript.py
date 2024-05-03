import serial
import serial_config as cfg
import sys
import msvcrt
import os

def readSerial():
	while True:
		line = ser.readline()
		try:
			if (int(line.strip()) == 0):
				break
		except:
			print(line.decode('utf-8').strip())


ser = serial.Serial(cfg.PORT, cfg.BAUDRATE, timeout=10)
print("Connected to PORT: ", cfg.PORT,"\n")

while(1):
	print(f"Which Test To Run?\nCommunications Test: c\nSpeed Test: s\nGet Vector: v\nQuit: <ESC>")

	key = msvcrt.getch()
	if key == b'c':
		print("Running Communications Test")
		ser.write(cfg.commands['RUN_COMMS_TEST'])
		readSerial()
		print("Finished Communications Test")

	elif key == b's':
		print("Running Speed Test")
		ser.write(cfg.commands['RUN_SPEED_TEST'])
		readSerial()
		print("Finished Speed Test")
	elif key == b'v':
		print("Getting Vector")
		ser.write(cfg.commands['PRINT_VECTOR'])
		line = ser.readline()
		print(line.decode('utf-8').strip())
		print("Got Vector")
	elif key == b'\x1b':
		break
