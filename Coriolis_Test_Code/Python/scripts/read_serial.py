import serial
import serial_config as cfg
import sys
import msvcrt
import time

# Adjust the port and baud rate based on your Arduino setup
ser = serial.Serial(cfg.PORT, cfg.BAUDRATE, timeout=1)

ser.flushInput()

# Reset the arduino count if program is running.
ser.write(b'\xFF')

# Semd the command to send data openended through the serial port
ser.write(b'\xAA')


try:
    with open('../data_files/read_serial.csv', 'w') as file:
        file.write("Reading_Count,Pitch,Yaw,Pitotstatic\n")


        while True:

            # Wait for user input for 1 second
            start_time = time.time()
            while (time.time() - start_time) < 1:
                if msvcrt.kbhit():
                    key = msvcrt.getch()
                    if key == b'\r':  # If user presses Enter
                        sys.exit()

            ser.write(b'\xF8')
            # Read a line from the serial port
            line = ser.readline().decode('utf-8')

            # Print to the console and write to a file
            print(line.strip())
            file.write(line.strip() + '\n')

finally:
    ser.write(b'\x00')
