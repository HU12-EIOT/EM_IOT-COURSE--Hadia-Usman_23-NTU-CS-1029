#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//OLED Setup
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Pin map like for what purpose pins are being used
#define LED_PIN    15    // for output Led pin
#define BUZZER_PIN 17     // for buzzer
#define BTN_PIN    32     // Button press 

// Variables declared also flag set 
unsigned long pressStart = 0; // to store button pressed state
bool buttonPressed = false; // flag to check button pressed state
bool ledState = false;//flag to check led state 

const unsigned long LONG_PRESS_TIME = 2000;  // 2 sec

//Display Text on OLED 
void updateOLED(const String &line1, const String &line2 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(line1);
  if (line2 != "") {
    display.setCursor(0, 30);
    display.println(line2);
  }
  display.display();
}

//Hanndling if button pressed for short time less than 2 sec 
// so Toggle Led  
void handleShortPress() {
  ledState = !ledState;
  digitalWrite(LED_PIN, ledState);
  Serial.println("Short Press → Toggle LED");
  updateOLED("Short Press", ledState ? "LED: ON" : "LED: OFF");
}
//Handle for long press like more than 2 sec or upto 2 sec so Play Buzzer
void handleLongPress() {
  Serial.println("Long Press → Play Buzzer");
  updateOLED("Long Press", "Buzzer Tone...");
  tone(BUZZER_PIN, 1000, 400);  // 1kHz tone for 400ms
  delay(400);
  noTone(BUZZER_PIN);
  updateOLED("Long Press", "Done");
}
// In setup done things and proivde input and output in pin mode 
void setup() {
  Serial.begin(115200);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  pinMode(LED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BTN_PIN, INPUT_PULLUP);
  //Intialize system 
  updateOLED("System Ready");
  Serial.println("System Ready");
}
//Loop Function  using if -else for button states
void loop() {
  int buttonState = digitalRead(BTN_PIN);

  //Button pressed (Low as a reult of using input_pullup)
  if (buttonState == LOW && !buttonPressed) {
    buttonPressed = true;
    pressStart = millis();
    updateOLED("Button Pressed...");
  }

  //When after pressing release button then check 
  // if long press or short press
  if (buttonState == HIGH && buttonPressed) {
    buttonPressed = false;
    unsigned long pressDuration = millis() - pressStart;

    if (pressDuration > LONG_PRESS_TIME) {
      handleLongPress();
    } else {
      handleShortPress();
    }
  }

  delay(20); //simple debounce delay
}