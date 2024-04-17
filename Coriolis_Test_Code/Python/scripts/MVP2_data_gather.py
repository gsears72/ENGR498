import serial
import serial_config as cfg
import sys
import msvcrt
import os

CONF = "Board1_ProbeB" 
DATE = "16APR2024"

def collect_data_and_write_file(speed, type, start_angle, end_angle, angle_step, file_prefix):
    current_angle = start_angle  # Initialize current angle

    while current_angle <= end_angle:
        # Prompt to press enter
        last_angle = current_angle - angle_step
        
        if current_angle != start_angle:
            print(f"Press <Enter> to collect {speed}%, {type}, {current_angle}°\t<Backspace> to redo {last_angle}°\t <ESC> to exit...")
        else:
            print(f"Press <Enter> to collect {type} {current_angle}°\t <ESC> to exit...")
        
        key = msvcrt.getch()
        if key == b'\r':  # If user presses Enter
            # Reset the arduino count if program is running.
            ser.write(cfg.RESET_SIGNAL)

            # Send the command to send data open-ended through the serial port
            ser.write(cfg.READ_SERIAL)

            # Define the base directory relative to the script
            base_directory = os.path.abspath(f"../../data_files/{CONF}_{DATE}")

            # Construct the directory path using string formatting
            directory_path = os.path.join(base_directory, file_prefix)

            # Check if the directory exists
            if not os.path.exists(directory_path):
                # Create the directory
                os.makedirs(directory_path)
            
            file_path = f"../../data_files/{CONF}_{DATE}/{file_prefix}/{current_angle}deg.csv"

            with open(file_path, 'w') as file:
                file.write("Reading_Count,Pitch,Yaw,Pitotstatic\n")

                for _ in range(200):
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

types_of_test = ["Yaw","Diagonal", "Static"]#["Pitch","Yaw","Diagonal", "Static"]

try:
    print("MVP2 MultiSpeed MultiAngle Test Program")
    start_angle = int(input("Enter start angle for test in °: "))
    end_angle = int(input("Enter end angle for test in °: "))
    angle_step = int(input("Enger angle step for test in °: "))
    initial_speed = int(input("Enter initial speed for test in mph: "))
    final_speed = int(input("Enter final speed in mph: "))
    speed_step = int(input("Enter speed step in mph: "))

    """
    print("\nStatic Test:\nEnsure tunnel speed set to 0mph\nPress <Enter> to gather static data\tPress <ESC> to exit")
    key = msvcrt.getch()
    if key == b'\r':  # If user presses Enter
        collect_data_and_write_file(speed = 0, type = "Static", start_angle = 0, end_angle = 0, angle_step = angle_step, file_prefix=f"0mph")
    elif key == b'\x1b':  # If user presses Escape
        print("Exiting the program...")
        sys.exit()

    else:
        print("Invalid input. Please press Enter or Backspace.")
    """

    for type in types_of_test:
        for current_speed in range(initial_speed,final_speed+1,speed_step):
            print("")
            if type == "Pitch":
                print(f"{current_speed}mph {type} Test:\nMove probe to pitch={start_angle}°, yaw=0°\nSet tunnel speed to {current_speed}mph\nPress <Enter> to start test\tPress <ESC> to exit")
            elif type == "Yaw":
                print(f"{current_speed}mph {type} Test:\nMove probe to pitch=0°, yaw={start_angle}°\nSet tunnel speed to {current_speed}mph\nPress <Enter> to start test\tPress <ESC> to exit")
            elif type == "Diagonal":
                print(f"{current_speed}mph {type} Test:\nMove probe to pitch={start_angle}°, yaw={start_angle}°\nSet tunnel speed to {current_speed}mph\nPress <Enter> to start test\tPress <ESC> to exit")
            else:
                print("Invalid Test Type")
    
            key = msvcrt.getch()
            if key == b'\r':  # If user presses Enter
                collect_data_and_write_file(speed = current_speed, type = type, start_angle = start_angle, end_angle = end_angle, angle_step = angle_step, file_prefix=f"{type}_{current_speed}mph")

            elif key == b'\x1b':  # If user presses Escape
                print("Exiting the program...")
                sys.exit()

            else:
                print("Invalid input. Please press Enter or Backspace.")
    
        print("--------------------------------")

finally:
    ser.write(b'\x00')
