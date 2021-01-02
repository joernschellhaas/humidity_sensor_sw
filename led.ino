#include <Adafruit_NeoPixel.h>

#define countof(var) (sizeof(var)/sizeof(var[0]))
#define COLOR_AT_INDEX(color, idx) *(((uint8_t*)&color) + idx)

typedef union {
  uint32_t value;
  uint8_t components[4];
} color_t;

struct color_mapping {
  float key;
  color_t color;
};
typedef struct color_mapping color_mapping_t;

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(/*NUMPIXELS=*/ 1, /*PIN=*/ D8, NEO_GRB + NEO_KHZ800);

color_mapping_t color_mappings[] = {
  {35, pixels.Color(0, 0, 127)},
  {40, pixels.Color(0, 95, 95)},
  {45, pixels.Color(0, 0, 0)},
  {55, pixels.Color(0, 0, 0)},
  {60, pixels.Color(191, 90, 0)},
  {65, pixels.Color(255, 0, 0)},
};

uint32_t interpolate(float input, struct color_mapping color_mappings[], size_t color_mappings_size) {
  color_t lower = color_mappings[0].color;
  color_t higher = color_mappings[color_mappings_size - 1].color;
  float dlower = 0;
  float dhigher = 0;
  
  // Find closest lower value
  for(size_t mi = 0; mi < color_mappings_size; ++mi) {
    if(color_mappings[mi].key <= input) {
      dlower = input - color_mappings[mi].key;
      lower = color_mappings[mi].color;
    }
  }

  // Find closest higher value
  for(size_t mi = color_mappings_size - 1; mi > 0; --mi) {
    if(color_mappings[mi].key >= input) {
      dhigher = color_mappings[mi].key - input;
      higher = color_mappings[mi].color;
    }
  }

  color_t color = {0};
  if(lower.value == higher.value) {
      return lower.value;
  }
  // Interpolate colors since there was not exact match
  for(int ci = 0; ci < 4; ++ci) {
    color.components[ci] = (dlower * higher.components[ci] + dhigher * lower.components[ci]) / (dhigher + dlower);
    DEBUG(Serial.println("lower[ci] = " + String(color.components[ci], HEX));)
    DEBUG(Serial.println("color[" + String(ci) + "]: " + String(color.components[ci], HEX));)
  }

  DEBUG(Serial.println("Lower: " + String(lower.value, HEX));)
  DEBUG(Serial.println("Higher: " + String(higher.value, HEX));)
  DEBUG(Serial.println("dLower: " + String(dlower));)
  DEBUG(Serial.println("dHigher: " + String(dhigher));)
  DEBUG(Serial.println("Color: " + String(color.value, HEX));)
  return color.value;
}

void setup_led() {
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
}

void update_led(float humidity ) {
  pixels.clear(); // Set all pixel colors to 'off'
  pixels.setPixelColor(0, interpolate(humidity, color_mappings, countof(color_mappings)));
  pixels.show();   // Send the updated pixel colors to the hardware.
}
