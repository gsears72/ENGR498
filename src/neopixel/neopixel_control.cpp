#include "neopixel.hpp"

Adafruit_NeoPixel pixels(NUMPIXELS, PIXELPIN, NEO_GRB + NEO_KHZ800);

void turnOnLED() {
	pixels.begin();    // Initialize the NeoPixel library.
	pixels.setPixelColor(0, pixels.Color(127, 0, 255)); // Set the color of the first pixel (purple color)
	pixels.setBrightness(16);
	pixels.show();  // Send the updated pixel colors to the hardware.
}

