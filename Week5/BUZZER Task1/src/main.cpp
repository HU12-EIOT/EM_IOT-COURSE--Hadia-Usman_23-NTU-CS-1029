#include <Arduino.h>

// ---------- Buzzer ----------
#define BUZZER_PIN  27
#define PWM_CH      0
#define FREQ        2000
#define RESOLUTION  10

// ---------- LED1 ----------
#define LED1_PIN 18
#define PWM1_CH 1
#define FREQ1 5000
#define RES1 8

// ---------- LED2 ----------
#define LED2_PIN 19
#define PWM2_CH 2
#define FREQ2 2000
#define RES2 8

// --- LED Task Function ---
void ledTask(void *parameter) {
  while (true) {
    // LED1 fade in/out
    for (int d = 0; d <= 255; d++) {
      ledcWrite(PWM1_CH, d);
      ledcWrite(PWM2_CH, 255 - d);  // opposite effect
      delay(5);
    }
    for (int d = 255; d >= 0; d--) {
      ledcWrite(PWM1_CH, d);
      ledcWrite(PWM2_CH, 255 - d);
      delay(5);
    }
  }
}

// --- Buzzer Task Function ---
void buzzerTask(void *parameter) {
  while (true) {
    // 1. Beep pattern
    for (int i = 0; i < 3; i++) {
      ledcWriteTone(PWM_CH, 2000 + i * 400);
      delay(150);
      ledcWrite(PWM_CH, 0);
      delay(150);
    }

    // 2. Frequency sweep
    for (int f = 400; f <= 3000; f += 100) {
      ledcWriteTone(PWM_CH, f);
      delay(20);
    }
    ledcWrite(PWM_CH, 0);
    delay(500);

    // 3. Melody
    int melody[] = {262, 330, 392, 523, 392, 330, 262, 330, 392, 330, 262};
    for (int i = 0; i < 11; i++) {
      ledcWriteTone(PWM_CH, melody[i]);
      delay(250);
    }

    ledcWrite(PWM_CH, 0);  // stop buzzer for a short break
    delay(1000);
  }
}

void setup() {
  // --- Setup PWM channels ---
  ledcSetup(PWM_CH, FREQ, RESOLUTION);
  ledcAttachPin(BUZZER_PIN, PWM_CH);

  ledcSetup(PWM1_CH, FREQ1, RES1);
  ledcAttachPin(LED1_PIN, PWM1_CH);

  ledcSetup(PWM2_CH, FREQ2, RES2);
  ledcAttachPin(LED2_PIN, PWM2_CH);

  // --- Create two concurrent tasks ---
  xTaskCreatePinnedToCore(
    ledTask,      // Task function
    "LED Task",   // Name
    2048,         // Stack size
    NULL,         // Parameter
    1,            // Priority
    NULL,         // Task handle
    1             // Run on core 1
  );

  xTaskCreatePinnedToCore(
    buzzerTask,   // Task function
    "Buzzer Task",
    2048,
    NULL,
    1,
    NULL,
    0             // Run on core 0
  );
}

void loop() {
  // Empty — tasks handle everything
}