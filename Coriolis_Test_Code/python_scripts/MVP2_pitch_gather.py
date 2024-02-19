import serial

# Adjust the port and baud rate based on your Arduino setup
ser = serial.Serial('COM19', 115200, timeout=1)

try:
    for pitch_deg in range(-25,25,1):

        print(f"Pitch: {pitch_deg}°")

        next = pitch_deg + 1

        # Reset the arduino count if program is running.
        ser.write(b'\xFF')

        # Semd the command to send data openended through the serial port
        ser.write(b'\xAA')


        file_path = f"../data_files/MVP2/Yaw_fast/{pitch_deg}deg.csv"

        with open(file_path, 'w') as file:
            file.write("Reading_Count,Pitch,Yaw,Pitotstatic\n")

            ser.flushInput()

            for _ in range(200):
                # Read a line from the serial port
                line = ser.readline().decode('utf-8')

                # Print to the console and write to a file
                print(line.strip())
                file.write(line.strip() + '\n')

        # Prompt to press enter
        input(f"Press Enter to continue to {next}...")

finally:
    print("Finished pitch gather.")
    ser.write(b'\x00')
