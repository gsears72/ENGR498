import numpy as np
import matplotlib.pyplot as plt
import matplotlib.animation as animation
from matplotlib.patches import FancyArrowPatch

# Function to update the graph data
def update(frame):
    global vector, arrow, vector_text
    # Update vector data (for demonstration, just rotating the vector)
    rotation_matrix = np.array([[np.cos(np.pi / 180), -np.sin(np.pi / 180)],
                                [np.sin(np.pi / 180), np.cos(np.pi / 180)]])
    vector = np.dot(rotation_matrix, vector)
    magnitude = 50 + 25 * np.sin(np.radians(frame))  # Varying magnitude between 25 to 75
    vector = magnitude * vector / np.linalg.norm(vector)  # Normalize vector with new magnitude
    arrow.set_positions((0, 0), (vector[0], vector[1]))  # Set new data for the arrow
    direction = np.arctan2(vector[1], vector[0]) * 180 / np.pi
    # Update text annotation
    vector_text.set_text(f"Direction: {direction:.2f}°\nMagnitude: {magnitude:.2f}")  
    return arrow, vector_text

# Initial vector data
vector = np.array([1, 0])

# Create figure and axis
fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(10, 5))

# Subplot 1: Graph
ax1.set_xlim(-80, 80)
ax1.set_ylim(-80, 80)
ax1.set_aspect('equal', 'box')
arrow = FancyArrowPatch((0, 0), (vector[0], vector[1]), color='green', mutation_scale=20)
ax1.add_patch(arrow)
# Add grid lines
ax1.grid(True, which='major', linestyle=':', linewidth='0.5', color='gray')
# Add axis titles
ax1.set_xlabel('Speed (mph)')
ax1.set_ylabel('Speed (mph)')

# Subplot 2: Readout
ax2.axis('off')  # Turn off axis for cleaner display
vector_text = ax2.text(0.5, 0.5, '', fontsize=12, horizontalalignment='left')

# Animation function
ani = animation.FuncAnimation(fig, update, frames=np.arange(0, 360), interval=10, blit=True)

# Show plot
plt.show()
