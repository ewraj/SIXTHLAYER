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
int scrollY = SCREEN_HEIGHT;

void setup() {
  // 115200 is fast enough for near-instant text transfer
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); 
  }
  
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setTextWrap(true);
}

void loop() {
  // 1. Check if the Mac Web Terminal sent us new AI text
  if (Serial.available() > 0) {
    String incoming = Serial.readStringUntil('\n'); // Read until newline
    incoming.trim();
    if (incoming.length() > 0) {
      hudText = incoming;
      scrollY = SCREEN_HEIGHT; // Reset scroll position to bottom for the new text
    }
  }

  // 2. Draw Frame
  display.clearDisplay();
  
  // Tactical Static Overlay (Corner Brackets)
  int len = 8;
  display.drawLine(0, 0, len, 0, SSD1306_WHITE);
  display.drawLine(0, 0, 0, len, SSD1306_WHITE);
  display.drawLine(127, 0, 127 - len, 0, SSD1306_WHITE);
  display.drawLine(127, 0, 127, len, SSD1306_WHITE);
  display.drawLine(0, 63, len, 63, SSD1306_WHITE);
  display.drawLine(0, 63, 0, 63 - len, SSD1306_WHITE);
  display.drawLine(127, 63, 127 - len, 63, SSD1306_WHITE);
  display.drawLine(127, 63, 127, 63 - len, SSD1306_WHITE);

  // 3. Draw Scrolling Text
  // Set X to 12 to keep it inside the tactical brackets
  // The y variable controls vertical scrolling
  display.setCursor(12, scrollY); 
  
  // Custom bounding box for wrap, Adafruit_GFX handles standard wrap at SCREEN_WIDTH
  // so we can't easily restrict the right margin unless we manage word wrap manually,
  // but standard wrap to 128 is fine for now.
  display.print(hudText);
  
  display.display();
  
  // 4. Advance scroll
  scrollY--;
  if (scrollY < -400) { // Large negative limit in case the AI writes a huge paragraph
    scrollY = SCREEN_HEIGHT; // Loop it back to the bottom
  }
  
  delay(40); // Scroll speed (smaller = faster)
}
