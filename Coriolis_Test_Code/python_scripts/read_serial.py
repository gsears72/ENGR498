import serial
import serial_config as cfg

# Adjust the port and baud rate based on your Arduino setup
ser = serial.Serial(cfg.PORT, cfg.BAUDRATE, timeout=1)

ser.flushInput()

# Reset the arduino count if program is running.
ser.write(b'\xFF')

# Semd the command to send data openended through the serial port
ser.write(b'\xAA')


try:
    with open('../data_files/test.csv', 'w') as file:
        file.write("Reading_Count,Pitch,Yaw,Pitotstatic\n")


        while True:
            ser.write(b'\xF8')
            # Read a line from the serial port
            line = ser.readline().decode('utf-8')

            # Print to the console and write to a file
            print(line.strip())
            file.write(line.strip() + '\n')

finally:
    ser.write(b'\x00')
