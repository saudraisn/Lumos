#include <Adafruit_NeoPixel.h>

#define PIN 6

const int NUM_LEDS = 300;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

String inputString = "";
char command = "";
boolean stringComplete = false;

void setup() {

  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  inputString.reserve(200);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // colorAll(0x29c086);

  // colorAll(strip.Color(96, 255, 231));
}

void loop() {
  // Some example procedures showing how to display to the pixels:
  // colorWipe(strip.Color(255, 0, 0), 50); // Red
  // colorWipe(strip.Color(0, 255, 0), 50); // Green
  // colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // // Send a theater pixel chase in...
  // theaterChase(strip.Color(127, 127, 127), 50); // White
  // theaterChase(strip.Color(127, 0, 0), 50); // Red
  // theaterChase(strip.Color(0, 0, 127), 50); // Blue

  // rainbow(5);
  // rainbowCycle(20);
  // theaterChaseRainbow(50);

  // colorRotate(30);
  // Serial.println(inputString);

  if (Serial.available()) {
    char read = (char)Serial.read();
    if (read != '\n') {
      command = read;
    }
  }

  if (command == 'B') {
    colorAll(0x0000ff);
  }
  else if (command == 'P') {
    strobe();
  }
  else if (command == 'S') {
    uint32_t c1 = 0xff01b7;
    uint32_t c2 = 0x9000ff;
    colorSwipe(c1, c2, 500);
    colorSwipe(c2, c1, 500);
  } else if (command == 'R') {
    colorRotate(1);
  }

  uint32_t c1 = 0xff01b7;
  uint32_t c2 = 0x9000ff;
  colorSwipe(c1, c2, 2000);
  colorSwipe(c2, c1, 2000);
}

void strobe() {
  colorAll(0x000000);
  delay(50);
  colorAll(0xffffff);
  delay(50);
}

void colorRotate(unsigned int del)  {
  for (int i = 0; i < 255; i++) {
    colorAll(Wheel(i));
    i++;
    i %= 255;
    delay(del);
  }
}

void colorSwipe(uint32_t c1, uint32_t c2, uint16_t time) {
  double r1 = (double) ((c1 >> 16) & 255);
  double g1 = (double) ((c1 >> 8) & 255);
  double b1 = (double) (c1 & 255);

  double r2 = (double) ((c2 >> 16) & 255);
  double g2 = (double) ((c2 >> 8) & 255);
  double b2 = (double) (c2 & 255);

  int numSteps = 50;

  double deltaR = (r2 - r1) / numSteps;
  double deltaG = (g2 - g1) / numSteps;
  double deltaB = (b2 - b1) / numSteps;

  double r = r1;
  double g = g1;
  double b = b1;

  for (int i = 0; i < numSteps; i++) {
    colorAll(strip.Color(round(r), round(g), round(b)));
    // Serial.println(String(round(r), HEX)+ String(round(g), HEX) + String(round(b), HEX));
    r += deltaR;
    g += deltaG;
    b += deltaB;
    delay(round((double)time / (double)numSteps));
  }
}

void colorAll(uint32_t color) {
  for (int i = 0 ; i < NUM_LEDS; i++) {
    strip.setPixelColor(i, color);
  }
  strip.show();
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256; j++) {
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j = 0; j < 10; j++) { //do 10 cycles of chasing
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, c);  //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j = 0; j < 256; j++) {   // cycle all 256 colors in the wheel
    for (int q = 0; q < 3; q++) {
      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, Wheel( (i + j) % 255)); //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i = 0; i < strip.numPixels(); i = i + 3) {
        strip.setPixelColor(i + q, 0);      //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if (WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
