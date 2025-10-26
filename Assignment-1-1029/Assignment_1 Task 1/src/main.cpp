#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED SETUP
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Pin Map means which Pin are using  for what purpose
#define LED_PIN1 19 // First LED_Pin 1
#define LED_PIN2 18 // Second LED_Pin 2
#define LED_PIN3 15  // PWM LED_PIN 3  (fading)
#define BTN1_MODE 4// Button 1 For mode change
#define BTN2_RESET 5 // Reset Button 

// Variables like volatile ,uint8_t type
volatile bool modeBtn1Pressed = false;
volatile bool resetBtn2Pressed = false;
//Debounce variable 
uint8_t mode = 0;         //current operating mode upto  0 to 3
unsigned long lastDebounce = 0; // last debounce time 
const unsigned long debounceDelay = 200; // debounce delay 
//Blink variable for alternate blink
unsigned long blinkTimer = 0;// Previous Blink value for blink control
const unsigned long blinkInterval = 500; // Blinl delay
//For PWM fade
int fadeValue = 0;// current brightness 0-255
int fadeDir = 1;//direction of fade as (1=up, -1=down)

// To Handle the ISR For t te function and Handle the button pressed debounce 
void handleMode() {
  unsigned long now = millis();
  if (now - lastDebounce > debounceDelay) {
    modeBtn1Pressed = true;
    lastDebounce = now;
  }
}
//For reset Button handle debounce 
void handleReset() {
  unsigned long now = millis();
  if (now - lastDebounce > debounceDelay) {
    resetBtn2Pressed = true;
    lastDebounce = now;
  }
}
//OLED SETUP To display Different Led states ON OLED
void updateOLED(const String &line1, const String &line2 = "",const String &line3 = "") {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  int yPos = 0;
  display.setCursor(0, yPos);
  display.println(line1);
  yPos += 15; // Move down for next line
  if (line2 != "") {
    display.setTextSize(1);  
    display.setCursor(0, yPos);
    display.println(line2);
    yPos += 15; // Move down to further next line 
  }
   if (line3 != "") {
    display.setTextSize(1);  
    display.setCursor(0, yPos);
   }
  display.display();
}
//Button pressed modes which result to different led states 
void setMode(uint8_t newMode) {
  mode = newMode;
  analogWrite(LED_PIN3, 0); // stop PWM on LED_PIN3
  switch (mode) {
    case 0: //First case 0 Both OFF
      digitalWrite(LED_PIN1, LOW);
      digitalWrite(LED_PIN2, LOW);
      updateOLED("Mode 0: Both Leds OFF");
      break;
    case 1: //Second case 1 Alternate Blink
      updateOLED("Mode 1: Alternate Blink for Leds");
      break;
    case 2: // Third case 2 both ON
      digitalWrite(LED_PIN1, HIGH);
      digitalWrite(LED_PIN2, HIGH);
      updateOLED("Mode 2: Both Leds ON");
      break;
    case 3: // Fouth case 3 For third Led PWM fade 
      updateOLED("Mode 3: LED3 PWM Fade");
      break;
  }
}
//SETUP
void setup() {
  Serial.begin(115200);
  Wire.begin();
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay();
  display.display();

  //Pin mode to proivde input or ouptut 
  pinMode(LED_PIN1, OUTPUT);
  pinMode(LED_PIN2, OUTPUT);
  pinMode(LED_PIN3, OUTPUT);
  pinMode(BTN1_MODE, INPUT_PULLUP);
  pinMode(BTN2_RESET, INPUT_PULLUP);
  //Attach interrupt funtcion call  state falling 
  attachInterrupt(digitalPinToInterrupt(BTN1_MODE), handleMode, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN2_RESET), handleReset, FALLING);
  //Intialize system 
  setMode(0);
  updateOLED("System Ready");
}

//Loop setup
void loop() {
  //Handling Both button states using if_else 
  if (modeBtn1Pressed) {
    modeBtn1Pressed = false;
    mode = (mode + 1) % 4;
    setMode(mode);//set states from o to 3
  }

  if (resetBtn2Pressed) {
    resetBtn2Pressed = false;
    // Show Reset message first
    updateOLED("Reset Button Pressed!", "Returning to Mode 0");
    delay(1500); // small delay so user can see message
    setMode(0);// set to 0 state 
  }

  //Led Behaviour handle through button like 
  //if case 1 so alternate blink 
  if (mode == 1) { // Alternate blink
    if (millis() - blinkTimer >= blinkInterval) {
      blinkTimer = millis();
      digitalWrite(LED_PIN1, !digitalRead(LED_PIN1));
      digitalWrite(LED_PIN2, !digitalRead(LED_PIN2));
    }
  } //if case 3 so LED_PIN 3 Fade 
  else if (mode == 3) { // PWM fade LED_PIN3
    fadeValue += fadeDir * 5;
    if (fadeValue >= 255 || fadeValue <= 0) fadeDir *= -1;
    analogWrite(LED_PIN3, fadeValue);
    delay(20);// smooth fading 
  }
  
}


