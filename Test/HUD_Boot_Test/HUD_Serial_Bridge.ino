#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Your verified pins
#define OLED_MOSI   13 // D7
#define OLED_CLK    14 // D5
#define OLED_DC     5  // D1
#define OLED_CS     15 // D8
#define OLED_RESET  4  // D2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

String hudText = "AWAITING NEURAL LINK...";
int charsToDraw = 0;
unsigned long lastDrawTime = 0;

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextWrap(true);
  
  charsToDraw = hudText.length();
}

void loop() {
  // 1. Check for new AI text
  if (Serial.available() > 0) {
    String incoming = Serial.readStringUntil('\n');
    incoming.trim();
    if (incoming.length() > 0) {
      hudText = incoming;
      charsToDraw = 0; // Reset typing animation
    }
  }

  // 2. Draw Frame
  display.clearDisplay();
  
  int len = 8;
  display.drawLine(0, 0, len, 0, SSD1306_WHITE);
  display.drawLine(0, 0, 0, len, SSD1306_WHITE);
  display.drawLine(127, 0, 127 - len, 0, SSD1306_WHITE);
  display.drawLine(127, 0, 127, len, SSD1306_WHITE);
  display.drawLine(0, 63, len, 63, SSD1306_WHITE);
  display.drawLine(0, 63, 0, 63 - len, SSD1306_WHITE);
  display.drawLine(127, 63, 127 - len, 63, SSD1306_WHITE);
  display.drawLine(127, 63, 127, 63 - len, SSD1306_WHITE);

  // 3. Typewriter effect logic
  if (charsToDraw < hudText.length()) {
    if (millis() - lastDrawTime > 25) { // 25ms per character typing speed
      charsToDraw++;
      lastDrawTime = millis();
    }
  }

  // 4. Draw Text
  // Start at x=10, y=14 to sit nicely inside the brackets
  display.setCursor(10, 14); 
  
  // Only print up to the current animated character count
  display.print(hudText.substring(0, charsToDraw));
  
  // Tactical blinking block cursor
  if (charsToDraw == hudText.length()) {
    if ((millis() / 500) % 2 == 0) {
      display.print("_");
    }
  } else {
    display.print("_"); // Solid cursor while typing
  }
  
  display.display();
}
