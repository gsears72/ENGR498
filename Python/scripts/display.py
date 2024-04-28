import serial
import serial_config as cfg
import pygame
import sys
import numpy as np
from OpenGL.GL import *
from OpenGL.GLUT import *
from OpenGL.GLU import *
import numpy as np
import serial

# Adjust the port and baud rate based on your Arduino setup
ser = serial.Serial(cfg.PORT, cfg.BAUDRATE, timeout=1)

ser.flushInput()

# Reset the arduino count if program is running.
ser.write(b'\xFF')

# Send command to run the communcations test code
ser.write(b'\xFA')


# Initialize Pygame
pygame.init()

# Set up screen dimensions
WIDTH, HEIGHT = 800, 600
screen = pygame.display.set_mode((WIDTH, HEIGHT))
pygame.display.set_caption("3D Vector Visualization")

# Colors
BLACK = (0, 0, 0)
WHITE = (255, 255, 255)
RED = (255, 0, 0)
GREEN = (0, 255, 0)
BLUE = (0, 0, 255)
GRAY = (150, 150, 150)

# Initialize clock
clock = pygame.time.Clock()

# Function to project 3D points to 2D
def project(point, fov, viewer_distance):
    x, y, z = point
    factor = fov / (viewer_distance + z)
    projected_x = int(x * factor + WIDTH / 2)
    projected_y = int(-y * factor + HEIGHT / 2)
    return (projected_x, projected_y)

# Function to draw arrow
def draw_arrow(screen, color, start_pos, end_pos):
    pygame.draw.line(screen, color, start_pos, end_pos, 2)
    pygame.draw.polygon(screen, color, (
        (end_pos[0], end_pos[1] - 5),
        (end_pos[0] + 10, end_pos[1] + 10),
        (end_pos[0] - 10, end_pos[1] + 10)
    ))


# Main loop
running = True
while running:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Clear the screen
    screen.fill(BLACK)

    # Read data from serial port
    try:
        line = ser.readline().decode('utf-8').strip()
        vector_data = [float(val) for val in line.split(',')]
        vector = np.array(vector_data)
    except Exception as e:
        print("Error reading from serial:", e)
        continue

# Define viewer distance and field of view
    viewer_distance = 100
    fov = 500

    # Draw axis lines
    pygame.draw.line(screen, GRAY, (WIDTH / 2, 0), (WIDTH / 2, HEIGHT), 1)
    pygame.draw.line(screen, GRAY, (0, HEIGHT / 2), (WIDTH, HEIGHT / 2), 1)

    # Project and draw vector
    projected_vector = project(vector, fov, viewer_distance)
    draw_arrow(screen, RED, (WIDTH / 2, HEIGHT / 2), projected_vector)

    # Update the display
    pygame.display.flip()

    # Cap the frame rate
    clock.tick(60)

# Close serial port
ser.close()

# Quit Pygame
pygame.quit()
sys.exit()
