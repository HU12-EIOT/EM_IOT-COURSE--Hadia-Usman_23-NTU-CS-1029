Wokwi Project Link:

https://wokwi.com/projects/445771426169086977

Loom Video Link:
https://www.loom.com/share/fc8756dcaa6643eb9209871b3cdaa4f0


//What is happening in the task 
In this we are using two buttons so we get multi modes of LED if we
press btn 1 so we get modes like Alternatte Blink ,Both ON,Both Off and PWM fade for LED 3 and if we press btn 2 so we get reset back to mode 0 means Off state all of this event happening is displayed in the OLED

//Libraries

Arduino ->for arduino basic functions like (Pin mode)
Wire.h -> for I2C Communicaton with OLED
Adafruit GFX -> for displaying text ,graphics
Adafruit SSD1306->for Oled screen control

//Pin Map
LED 1
 Pin 19/GPIO 19 -> used for both on and off or alternate blink on specifc modes 
LED 2
 Pin 18/GPIO 18 -> Also used for both on and off or alternate blink on specific modes
LED 3
 Pin 15/GPIO 15 -> used for brightness fading eefct with the help of PWM
BTN-1 Mode Button
Pin4/GPIO 4 -> It chnages the LED cycle mode from 0 to 3
BTN-2 Reset Button
Pin5 /GPIO 5 ->It Reset the system back to mode 0
I²C Pins 
Pin 21 /SDA and PIN 22-> the OLED display for showing mode and system status.

Logical Flow
The system start with mode 0 state means leds off
Then when pressed button 1 so mode chnages into 
Both OFF
Alternate Blink 
Both On
PWM fade for LED 3

After it Button 2 is pressed for reset state which reset the state to mode 0
All of these events hapening is displayed on OLED Screen


