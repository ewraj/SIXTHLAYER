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

void typeText(String text, int x, int y, int delayMs) {
  display.setCursor(x, y);
  for(int i = 0; i < text.length(); i++) {
    display.print(text[i]);
    display.display();
    delay(delayMs);
  }
}

void setup() {
  Serial.begin(115200);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  
  // 1. TACTICAL BOOT SEQUENCE
  delay(1000);
  typeText("SIXTHLAYER v1.0", 0, 0, 30);
  delay(400);
  
  typeText("NEURAL LINK: EST...", 0, 12, 20);
  delay(600);
  
  display.fillRect(0, 12, 128, 8, SSD1306_BLACK); // Clear "EST..." line
  typeText("NEURAL LINK: ACTV", 0, 12, 5);
  display.display();
  delay(300);
  
  typeText("LOADING VISION CORE", 0, 24, 20);
  
  // Glitchy progress bar
  display.drawRect(0, 36, 128, 6, SSD1306_WHITE);
  for(int i = 0; i < 124; i += (random(2, 6) * 2)) {
    display.fillRect(2, 38, i, 2, SSD1306_WHITE);
    display.display();
    delay(random(10, 50));
  }
  display.fillRect(2, 38, 124, 2, SSD1306_WHITE);
  display.display();
  delay(300);
  
  // Tactical flash
  display.invertDisplay(true);
  delay(150);
  display.invertDisplay(false);
  display.clearDisplay();

  // 2. HUD MOCKUP MAIN LOOP CONFIG
  buildHUD();
}

void buildHUD() {
  display.clearDisplay();
  
  // Draw corner brackets for tactical framing
  int len = 8;
  // Top-Left
  display.drawLine(0, 0, len, 0, SSD1306_WHITE);
  display.drawLine(0, 0, 0, len, SSD1306_WHITE);
  // Top-Right
  display.drawLine(127, 0, 127 - len, 0, SSD1306_WHITE);
  display.drawLine(127, 0, 127, len, SSD1306_WHITE);
  // Bottom-Left
  display.drawLine(0, 63, len, 63, SSD1306_WHITE);
  display.drawLine(0, 63, 0, 63 - len, SSD1306_WHITE);
  // Bottom-Right
  display.drawLine(127, 63, 127 - len, 63, SSD1306_WHITE);
  display.drawLine(127, 63, 127, 63 - len, SSD1306_WHITE);
  
  // Center Crosshair
  display.drawCircle(64, 32, 6, SSD1306_WHITE);
  display.drawLine(64, 20, 64, 24, SSD1306_WHITE); // top dash
  display.drawLine(64, 40, 64, 44, SSD1306_WHITE); // bottom dash
  display.drawLine(52, 32, 56, 32, SSD1306_WHITE); // left dash
  display.drawLine(72, 32, 76, 32, SSD1306_WHITE); // right dash
  
  // Telemetry Text
  display.setTextSize(1);
  display.setCursor(2, 2);
  display.print("V-CORE: ON");
  
  // Mocking the <20 char output
  display.setCursor(2, 54);
  display.print("STBY: AWAITING OBJ");
  
  display.display();
}

void loop() {
  // Create a subtle breathing/scanning effect on the center dot
  display.drawPixel(64, 32, SSD1306_WHITE);
  display.display();
  delay(600);
  
  display.drawPixel(64, 32, SSD1306_BLACK);
  display.display();
  delay(600);
}
