#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

// --- Pin configuration ---
#define LDR_PIN 34         // LDR analog pin
#define DHTPIN 14          // DHT11 data pin
#define DHTTYPE DHT11     // Change to DHT11 if needed

#define SDA_PIN 21         // I2C SDA
#define SCL_PIN 22         // I2C SCL

// --- OLED setup ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- DHT sensor setup ---
DHT dht(DHTPIN, DHTTYPE);

// --- Setup function ---
void setup() {
  Serial.begin(115200);
  Serial.println("ESP32 LDR + DHT22 Monitor");

  // Initialize I2C on custom pins
  Wire.begin(SDA_PIN, SCL_PIN);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("SSD1306 allocation failed");
    for (;;);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Initializing sensors...");
  display.display();

  // Initialize DHT sensor
  dht.begin();

  delay(2000);
}

// --- Main loop ---
void loop() {
  // --- Read LDR ---
  int adcValue = analogRead(LDR_PIN);
  float voltage = (adcValue / 4095.0) * 3.3;

  // --- Read Temperature and Humidity ---
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // --- Check for read errors ---
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Error reading DHT11 sensor!");
    return;
  }

  // --- Serial output ---
  Serial.printf("LDR: %d | Voltage: %.2f V | Temp: %.1f °C | Humidity: %.1f %%\n",
                adcValue, voltage, temperature, humidity);

  // --- Display on OLED ---
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Sensor Readings:");

  display.setCursor(0, 16);
  display.print("Temp: ");
  display.print(temperature, 1);
  display.println(" C");

  display.setCursor(0, 28);
  display.print("Humidity: ");
  display.print(humidity, 1);
  display.println(" %");

  display.setCursor(0, 40);
  display.print("LDR ADC: ");
  display.println(adcValue);

  display.setCursor(0, 52);
  display.print("Voltage: ");
  display.print(voltage, 2);
  display.println(" V");

  display.display();

  delay(2000); // update every 2 seconds
}