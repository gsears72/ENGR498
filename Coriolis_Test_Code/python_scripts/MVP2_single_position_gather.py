import serial
import serial_config as cfg

# Adjust the port and baud rate based on your Arduino setup
ser = serial.Serial(cfg.PORT, cfg.BAUDRATE, timeout=1)

try:
    input("Single Angle Test:\npress <Enter> to start collecting")

    pitch_deg = 25

    print(f"Pitch: {pitch_deg}°")

    next = pitch_deg + 1

    # Reset the arduino count if program is running.
    ser.write(cfg.RESET_SIGNAL)

    # Semd the command to send data openended through the serial port
    ser.write(cfg.READ_SERIAL)

    file_path = f"../data_files/MVP2/Yaw_slow/{pitch_deg}deg.csv"

    with open(file_path, 'w') as file:
        file.write("Reading_Count,Pitch,Yaw,Pitotstatic\n")

        ser.flushInput()

        for _ in range(200):
            # Read a line from the serial port
            ser.write(cfg.GET_DATA)
            line = ser.readline().decode('utf-8')

            # Print to the console and write to a file
            print(line.strip())
            file.write(line.strip() + '\n')


finally:
    print("Finished data gather.")
    ser.write(b'\x00')
