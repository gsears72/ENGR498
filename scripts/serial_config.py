# serial_config.py

import serial.tools.list_ports

def find_microcontroller_ports():
    ports = serial.tools.list_ports.comports()
    microcontroller_ports = []

    for port in ports:
        is_microcontroller = False  # Flag to determine if a port is a microcontroller
        #print(f"Port: {port.device}")
        #print(f" - Description: {port.description}")
        #print(f" - HWID: {port.hwid}")

        # List of conditions to identify different microcontrollers
        conditions = [
            "Arduino" in port.description,
            "USB Serial" in port.description,
            "VID:PID=239A:80CB" in port.hwid,  # Example for Adafruit boards
            "VID:PID=1A86:7523" in port.hwid,  # Example for CH340 chips
            "STM32" in port.description,  # Example for STM32 boards
        ]

        # Check if any condition is true
        if any(conditions):
            is_microcontroller = True

        if is_microcontroller:
            microcontroller_ports.append(port.device)
            #print(" -> Microcontroller found!")
            
    print("Microcontroller Ports Found:", microcontroller_ports)

    return microcontroller_ports

def parse_header_to_dict(header_path):
    commands = {}
    start_parsing = False  # Flag to start parsing after the specific comment is found

    with open(header_path, 'r') as file:
        for line in file:
            line = line.strip()
            
            # Check for the specific comment to start parsing after
            if "// Serial Communication Codes (Only insert commands below this line)" in line:
                start_parsing = True
                continue  # Skip the comment line itself
            
            # Start parsing only after the specific comment is found
            if start_parsing:
                if line.startswith('#define'):
                    parts = line.split()
                    if len(parts) == 3:
                        # Storing command and its value as bytes in the dictionary
                        command_value = int(parts[2], 16)  # Convert hex to int first
                        if command_value <= 0xFF:
                            commands[parts[1]] = command_value.to_bytes(1, 'little')  # Convert integer to a byte
                        else:
                            # Handle cases where the value is more than one byte if needed
                            byte_length = (command_value.bit_length() + 7) // 8  # Calculate the necessary bytes
                            commands[parts[1]] = command_value.to_bytes(byte_length, 'little')

    return commands



# Define the serial port variable
PORT = find_microcontroller_ports()[0]  # Example port, replace it with your actual port
BAUDRATE = 115200

commands = parse_header_to_dict("../include/serial_config.hpp")
