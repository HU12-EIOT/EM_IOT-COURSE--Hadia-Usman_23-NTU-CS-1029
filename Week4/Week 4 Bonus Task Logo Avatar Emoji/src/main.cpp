
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --------------- SIMPLE 16x16 BITMAPS ----------------

// Simple circle (logo)
const unsigned char logo_simple [] PROGMEM = {
  0x03,0xC0,
  0x0F,0xF0,
  0x1F,0xF8,
  0x3C,0x3C,
  0x38,0x1C,
  0x78,0x1E,
  0x70,0x0E,
  0x70,0x0E,
  0x70,0x0E,
  0x70,0x0E,
  0x78,0x1E,
  0x38,0x1C,
  0x3C,0x3C,
  0x1F,0xF8,
  0x0F,0xF0,
  0x03,0xC0
};

// Simple avatar (head & shoulders)
const unsigned char avatar_simple [] PROGMEM = {
  0x00,0x00,
  0x03,0xC0,
  0x07,0xE0,
  0x0E,0x70,
  0x0E,0x70,
  0x0E,0x70,
  0x07,0xE0,
  0x03,0xC0,
  0x0F,0xF0,
  0x1F,0xF8,
  0x1F,0xF8,
  0x1F,0xF8,
  0x1F,0xF8,
  0x1F,0xF8,
  0x00,0x00,
  0x00,0x00
};

// Simple smile emoji
const unsigned char emoji_simple [] PROGMEM = {
  0x03,0xC0,
  0x0F,0xF0,
  0x1F,0xF8,
  0x3C,0x3C,
  0x38,0x1C,
  0x71,0x8E, // eyes
  0x71,0x8E,
  0x70,0x0E,
  0x70,0x0E,
  0x78,0x1E,
  0x3C,0x3C,
  0x1F,0xF8,
  0x0F,0xF0,
  0x07,0xE0,
  0x00,0x00,
  0x00,0x00
};

// ---------------------------------------------------

void setup() {
  Wire.begin(21, 22);  // SDA, SCL
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    for (;;);
  }
  display.clearDisplay();
  display.display();
}

void showImageWithLabel(const unsigned char* image, const char* label) {
  display.clearDisplay();

  // Draw 16x16 bitmap at center
  int x = (SCREEN_WIDTH - 16) / 2;
  int y = 12;
  display.drawBitmap(x, y, image, 16, 16, SSD1306_WHITE);

  // Text below image
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int textX = (SCREEN_WIDTH - (strlen(label) * 6)) / 2;
  display.setCursor(textX, 40);
  display.println(label);

  display.display();
  delay(3000);
}

void loop() {
  showImageWithLabel(logo_simple, "LOGO");
  showImageWithLabel(avatar_simple, "AVATAR");
  showImageWithLabel(emoji_simple, "EMOJI");
}