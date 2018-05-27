/*  Connects to the home WiFi network
    Asks some network parameters
    Starts WiFi server with fix IP and listens
    Receives and sends messages to the client
    Communicates: wifi_client_01.ino
*/
#include <Adafruit_NeoPixel.h>
#include <ESP8266WiFi.h>

#define PIN D6

const int NUM_LEDS = 300;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);


byte ledPin = 2;
char ssid[] = "BELL022";               // SSID of your home WiFi
char pass[] = "4C2EC5AC";               // password of your home WiFi
WiFiServer server(80);

IPAddress ip(192, 168, 2, 177);            // IP address of the server
IPAddress gateway(192, 168, 0, 1);        // gateway of your network
IPAddress subnet(255, 255, 255, 0);       // subnet mask of your network

void setup() {
  Serial.begin(115200);                   // only for debug
  WiFi.config(ip, gateway, subnet);      // forces to use the fix IP
  WiFi.begin(ssid, pass);                 // connects to the WiFi router
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  server.begin();                         // starts the server
  Serial.println("Connected to wifi");
  Serial.print("Status: "); Serial.println(WiFi.status());  // some parameters from the network
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  //  Serial.print("Networks: "); Serial.println(WiFi.scanNetworks());

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  //  colorAll(0x29c086);
}

void loop () {
  WiFiClient client = server.available();
  if (client) {
    if (client.connected()) {
      client.setNoDelay(true);
      Serial.print("Client connected: ");
      String request = client.readStringUntil('\r');    // receives the message from the client

      handleRequest(request);

      Serial.print("From client: ");
      Serial.println(request);
      client.flush();
      client.println("Hi client! No, I am listening.\r"); // sends the answer to the client
    }
    client.stop();                // tarminates the connection with the client
  }
}

void handleRequest(String request) {
  if (request == "yellow") {
    Serial.println("Yellow command received");
    colorAll(0xffcc00);
  } else if (request == "blue") {
    Serial.println("Blue command received");
    colorAll(0x0000FF);
  } else {
    Serial.println("Unknown command received");
  }
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

