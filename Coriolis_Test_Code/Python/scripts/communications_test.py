import serial
import serial_config as cfg

# Adjust the port and baud rate based on your Arduino setup
ser = serial.Serial(cfg.PORT, cfg.BAUDRATE, timeout=1)

ser.flushInput()

# Reset the arduino count if program is running.
ser.write(b'\xFF')

# Send command to run the communcations test code
ser.write(b'\xFA')

try:
    with open('../data_files/test.csv', 'w') as file:
        file.write("Status Register, 24 - bit Sensor data, Digital Pressure Counts, Percentage of full scale pressure, Pressure Output, Temperature\n")

        while True:
            # Read a line from the serial port
            line = ser.readline().decode('utf-8')

            if (line.strip() == "end of test"):
                break

            # Print to the console and write to a file
            print(line.strip())
            file.write(line)

        print("End of Communications Test")
    
finally:
    ser.write(b'\x00')