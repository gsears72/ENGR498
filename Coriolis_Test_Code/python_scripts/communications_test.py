import serial

# Adjust the port and baud rate based on your Arduino setup
ser = serial.Serial('COM19', 115200, timeout=1)

# Reset the arduino count if program is running.
ser.write(b'\xFF')

# Send command to run the communcations test code
ser.write(b'\xFA')

try:
    with open('../data_files/test.csv', 'w') as file:
        file.write("Reading_Count,Pitch,Yaw,Pitotstatic\n")

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