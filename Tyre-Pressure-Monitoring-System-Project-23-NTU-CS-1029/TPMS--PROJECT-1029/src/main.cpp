// ------------TPMS-PROJECT-------------------
// Adding Blynk Template_Id so it can identiify the Template widget with which the 
// ESP-32 to coonect , Template-Name Human read form , Template_Auth_Token so
// it knows with which specific ESP-32 device i have to connect it

#define BLYNK_TEMPLATE_ID "TMPL666EqW8aD"
#define BLYNK_TEMPLATE_NAME "IoT Project"
#define BLYNK_AUTH_TOKEN "uBZj-A_paaaeospDaaen9jx9_gwCPaWZ"

// Connect Blynk with serial-monitor
#define BLYNK_PRINT Serial

// Librarires requires in project included

#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------------- Pins_Defined----------------
#define LED_PIN     4
#define Buzzer_PIN  18

// ---------------- WiFi_Conection ----------------
// Connecting ESP-32 with WIFI by proividng WIFI NAME and Password
char ssid[] = "Redmi11";
char pass[] = "tfgwzf8mqjqqq5e";

// ---------------- OLED_Display_Settings----------------
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
// Display object create passing speciifc parameters
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ---------------- BMP280_Pressure Sensor Object_create---------------
Adafruit_BMP280 bmp;

// ---------------- Pressure Logic  ----------------
float normal_temperature = 0;
const float base_psi = 14.7;
const float warning_appear = 14.5;
const float warning_disappears = 14.6;
const float multiplier = 3.0;

bool LOWwarning = false;

// ---------------- Timers----------------
// For counting the time if pressure below safe limit or at normal and send warning
// or stable msg after 3 minutes

const unsigned long THREE_MIN = 180000;

unsigned long lowStartTime = 0;
unsigned long stableStartTime = 0;

bool lowMsgSent = false;
bool stableMsgSent = false;

// ---------------- Alert Control ----------------
// Led and Buzzer on During Low pressure for 3 seconds and off for 8 seconds 
// and repeast it if still low
unsigned long alertStartTime = 0;
const unsigned long ALERT_DURATION = 3000;
const unsigned long ALERT_REPEAT_DELAY = 8000;
bool alertActive = false;

// ---------------- OLED Control ----------------
bool oledFrozen = false;
unsigned long lastOledUpdate = 0;

// ---------------- Blynk  for Scheduling events----------------
BlynkTimer timer;

//-------------------FUNCTION- Create-------------------------------

void readDisplayAndSend() {

  // -------- Read Temperature  --------
  float temperature = bmp.readTemperature();
  // if any of these given coniditon true stop cycle
  if (isnan(temperature) || temperature < -20 || temperature > 60) {
    Serial.println("BMP280 glitch detected  skipping cycle");
    return;
  }
  // First vald temperature
  if (normal_temperature == 0) {
    normal_temperature = temperature;
  }
// Calculate pseudo_pressure
  float pseudo_pressure =
    base_psi + (temperature - normal_temperature) * multiplier;

  // -------- LOW / STABLE Logic --------
  // Pressure is table or becomes stable after warning so it disppaers 
  if (LOWwarning) {
    if (pseudo_pressure >= warning_disappears) {
      LOWwarning = false;
      oledFrozen = false;
    }
  }
     // Pressure lOw warning appears
  else {
    if (pseudo_pressure <= warning_appear) {
      LOWwarning = true;
    }
  }

  // -------- LED & BUZZER --------
  // At low pressure Led and buzzer on for 3 seconds then off for 8 seconds
  // Then check if pressure still low so on  repeat cycle of on and off
  unsigned long now = millis();

  if (LOWwarning) {
    if (!alertActive && (now - alertStartTime >= ALERT_REPEAT_DELAY)) {
      alertActive = true;
      alertStartTime = now;
      digitalWrite(LED_PIN, HIGH);
      digitalWrite(Buzzer_PIN, HIGH);
    }
    // Pressure stable so Led and buzzer OFF
    if (alertActive && (now - alertStartTime >= ALERT_DURATION)) {
      digitalWrite(LED_PIN, LOW);
      digitalWrite(Buzzer_PIN, LOW);
      alertActive = false;
    }
  } // Pressure return to normal again so led and buzzer off
  else {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(Buzzer_PIN, LOW);
    alertActive = false;
    alertStartTime = 0;
  }

  // -------- OLED UPDATE CONTROL --------
  if (LOWwarning && millis() - lastOledUpdate < 4000) {
    return;   // reduce I2C stress during alert
  }
  lastOledUpdate = millis();
  // Values Display on Oled
  if (!(LOWwarning && oledFrozen)) {

    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);

    display.setCursor(0, 0);
    display.println("TPMS Monitor");
    display.println("----------------");

    display.print("Pressure: ");
    display.print(pseudo_pressure, 2);
    display.println(" PSI");

    display.print("Temp: ");
    display.print(temperature, 1);
    display.println(" C");
    //Warning Display on oled
    if (LOWwarning) {
      display.setCursor(0, 48);
      display.println("WARNING: LOW PSI!");
      oledFrozen = true;
    }

    display.display();
  }

  // -------- Blynk-Real Time Values Display--------
  Blynk.virtualWrite(V0, pseudo_pressure);
  Blynk.virtualWrite(V1, temperature);
  Blynk.virtualWrite(V2, LOWwarning ? 1 : 0);

  unsigned long currentTime = millis();

  // -------- Notification Logic --------
  // If low for 3 minute send notifction on Blynk and Gmail
if (pseudo_pressure <= warning_appear) {
  // -------- LOW PRESSURE --------
  stableStartTime = 0;
  stableMsgSent = false;

  if (lowStartTime == 0) {
    lowStartTime = currentTime;
  }

  if ((currentTime - lowStartTime >= THREE_MIN) && !lowMsgSent) {
    Blynk.logEvent("pressure_low", "Warning: Pressure is LOW!");
    lowMsgSent = true;
  }

} else {
  // -------- STABLE PRESSURE --------
  // If stable pressure and for 3 minutes son send notfication on Gmail and Blynk app 
  lowStartTime = 0;
  lowMsgSent = false;

  if (stableStartTime == 0) {
    stableStartTime = currentTime;
  }

  if ((currentTime - stableStartTime >= THREE_MIN) && !stableMsgSent) {
    Blynk.logEvent("pressure_stable", "Pressure is stable");
    stableMsgSent = true;
  }
}

  

  // -------- Serial_Monitor --------
  Serial.print("Pressure: ");
  Serial.print(pseudo_pressure);
  Serial.print(" PSI | Temp: ");
  Serial.print(temperature);
  Serial.println(" C");
}

//--------------SETUP-----------------------------
// Intialize Pins , Oled ,Pressure sensor and Blynk in setup()
void setup() {

  Serial.begin(115200);

  pinMode(LED_PIN, OUTPUT);
  pinMode(Buzzer_PIN, OUTPUT);

  Wire.begin(21, 22);
  Wire.setClock(100000);   

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init failed");
    while (1);
  }

  display.ssd1306_command(SSD1306_SETCONTRAST);
  display.ssd1306_command(0xFF);

  if (!bmp.begin(0x76)) {
    Serial.println("BMP280 init failed");
    while (1);
  }

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(2000L, readDisplayAndSend);
}


//---------LOOP------------------------

void loop() {
  Blynk.run();
  timer.run();
}


