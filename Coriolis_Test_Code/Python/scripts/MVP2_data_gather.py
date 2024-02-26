import serial
import serial_config as cfg
import sys
import msvcrt

def collect_data_and_write_file(type, start_angle, end_angle, angle_step, file_prefix):
    current_angle = start_angle  # Initialize current angle

    while current_angle <= end_angle:
        # Prompt to press enter
        last_angle = current_angle - 1
        if current_angle != start_angle:
            print(f"Press <Enter> to collect {type} {current_angle}°\t<Backspace> to redo {last_angle}°\t <ESC> to exit...")
        else:
            print(f"Press <Enter> to collect {type} {current_angle}°\t <ESC> to exit...")
        
        key = msvcrt.getch()
        if key == b'\r':  # If user presses Enter
            # Reset the arduino count if program is running.
            ser.write(cfg.RESET_SIGNAL)

            # Send the command to send data open-ended through the serial port
            ser.write(cfg.READ_SERIAL)

            file_path = f"../data_files/MVP2/{file_prefix}/{current_angle}deg.csv"

            with open(file_path, 'w') as file:
                file.write("Reading_Count,Pitch,Yaw,Pitotstatic\n")

                for _ in range(3):
                    # Read a line from the serial port
                    ser.flushInput()
                    ser.write(cfg.GET_DATA)
                    line = ser.readline().decode('utf-8')

                    # Print to the console and write to a file
                    print(line.strip())
                    file.write(line.strip() + '\n')

            current_angle += angle_step  # Move to the next angle

        elif key == b'\x08':  # If user presses Backspace
            if current_angle > start_angle:  # Make sure not to go below the start angle
                current_angle -= angle_step  # Move back one angle
                print(f"Redoing {type} {current_angle}°...")

        elif key == b'\x1b':  # If user presses Escape
            print("Exiting the program...")
            sys.exit()

        else:
            print("Invalid input. Please press Enter or Backspace.")




# Adjust the port and baud rate based on your Arduino setup
ser = serial.Serial(cfg.PORT, cfg.BAUDRATE, timeout=1)

try:
    print("Slow Speed Pitch Test:\nMove probe to pitch=-25°, yaw=0°\nSet tunnel speed to 20%\nPress <Enter> to start test\tPress <ESC> to exit")
    key = msvcrt.getch()
    if key == b'\r':  # If user presses Enter
        collect_data_and_write_file(type="pitch", start_angle=-25, end_angle=-23, angle_step=1, file_prefix="Pitch_slow")

    elif key == b'\x1b':  # If user presses Escape
        print("Exiting the program...")
        sys.exit()

    else:
        print("Invalid input. Please press Enter or Backspace.")
    
    print("--------------------------------")

    input("Slow Speed Yaw Test:\nMove probe to pitch=0°, yaw=-25° and press <Enter> to start test")
    key = msvcrt.getch()
    if key == b'\r':  # If user presses Enter
        collect_data_and_write_file(type="yaw", start_angle=-25, end_angle=-23, angle_step=1, file_prefix="Yaw_slow")

    elif key == b'\x1b':  # If user presses Escape
        print("Exiting the program...")
        sys.exit()

    else:
        print("Invalid input. Please press Enter or Backspace.")
    
    print("--------------------------------")

    input("Slow Speed Diagonal Test:\nMove probe to pitch=-25°, yaw=-25° and press <Enter> to start test")
    key = msvcrt.getch()
    if key == b'\r':  # If user presses Enter
        collect_data_and_write_file(type="diagonal", start_angle=-25, end_angle=23, angle_step=1, file_prefix="Diagonal_slow")

    elif key == b'\x1b':  # If user presses Escape
        print("Exiting the program...")
        sys.exit()

    else:
        print("Invalid input. Please press Enter or Backspace.")
    
    print("--------------------------------")

    input("Fast Speed Pitch Test:\nMove probe to pitch=-25°, yaw=0° and press <Enter> to start test")
    key = msvcrt.getch()
    if key == b'\r':  # If user presses Enter
        collect_data_and_write_file(type="pitch", start_angle=-25, end_angle=23, angle_step=1, file_prefix="Pitch_fast")

    elif key == b'\x1b':  # If user presses Escape
        print("Exiting the program...")
        sys.exit()

    else:
        print("Invalid input. Please press Enter or Backspace.")
    
    print("--------------------------------")

    input("Fast Speed Yaw Test:\nMove probe to pitch=0°, yaw=-25° and press <Enter> to start test")
    key = msvcrt.getch()
    if key == b'\r':  # If user presses Enter
        collect_data_and_write_file(type="yaw", start_angle=-25, end_angle=25, angle_step=1, file_prefix="Yaw_fast")

    elif key == b'\x1b':  # If user presses Escape
        print("Exiting the program...")
        sys.exit()

    else:
        print("Invalid input. Please press Enter or Backspace.")
    
    print("--------------------------------")

    input("Fast Speed Diagonal Test:\nMove probe to pitch=-25°, yaw=-25° and press <Enter> to start test")
    key = msvcrt.getch()
    if key == b'\r':  # If user presses Enter
        collect_data_and_write_file(type="diagonal", start_angle=-25, end_angle=23, angle_step=1, file_prefix="Diagonal_fast")

    elif key == b'\x1b':  # If user presses Escape
        print("Exiting the program...")
        sys.exit()

    else:
        print("Invalid input. Please press Enter or Backspace.")
    
    print("--------------------------------")
    
    print("Finished data gather.")

finally:
    ser.write(b'\x00')
