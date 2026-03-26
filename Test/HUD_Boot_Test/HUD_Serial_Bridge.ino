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

// Variables for smart scrolling
int scrollY = 14; 
int textHeightLines = 0;
bool isTypingDone = false;
unsigned long typingFinishedAt = 0;

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
  // 1. Check for NEW text over Serial
  if (Serial.available() > 0) {
    String incoming = Serial.readStringUntil('\n');
    incoming.trim();
    if (incoming.length() > 0) {
      hudText = incoming;
      charsToDraw = 0; 
      scrollY = 14; // Start back at top position
      isTypingDone = false;
    }
  }

  // 2. Draw Frame Static Layer
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

  // 3. Typewriting Logic
  if (charsToDraw < hudText.length()) {
    if (millis() - lastDrawTime > 25) { 
      charsToDraw++;
      lastDrawTime = millis();
    }
  } else if (!isTypingDone) {
    isTypingDone = true;
    typingFinishedAt = millis();
    
    // Estimate raw line height (roughly 22 characters per line with x=10 starting pos)
    // 128 (width) - 20 (padding) = 108 pixels width. 108 / 6 pixels per char = ~18 chars.
    textHeightLines = (hudText.length() / 18) + 1;
  }

  // 4. Scrolling logic (only if the text is longer than 5 lines / approx 35 pixels)
  // If text is long, start scrolling after 1 second of pause
  if (isTypingDone && textHeightLines > 4 && millis() - typingFinishedAt > 1000) {
     scrollY--;
     // If the whole block has scrolled off, reset
     if (scrollY < -(textHeightLines * 10)) {
        scrollY = 14; 
        typingFinishedAt = millis(); // Pause again at top
     }
  }

  // 5. Render
  display.setCursor(10, scrollY); 
  display.print(hudText.substring(0, charsToDraw));
  
  if (!isTypingDone || (millis() / 500) % 2 == 0) {
    display.print("_");
  }
  
  display.display();
  
  // Optional: slowdown scroll if we are in scroll mode
  if (isTypingDone && textHeightLines > 4) {
    delay(40);
  }
}
