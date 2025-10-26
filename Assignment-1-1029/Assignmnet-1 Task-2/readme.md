Wokwi Project Link :
https://wokwi.com/projects/445792656572830721

Loom Video Link:
https://www.loom.com/share/81de8233dc9948acb81aef14bec43921


//What is happening in the task 

In our this code we have to make smart button press detection system which detect if button pressed is long pressed or short pressed and after determing what type of press it was it activates either LED or the Buzzer

//Libraries
First we used libraries like arduino , adafruit gfx library for text display , Wire .<h> fir I2C communication and adfruit SSD_1306 for OLED control

//Pin Map 

//LED
Pin 15 /GPIO 15 -> It proivdes LED OUTPUT (Toggled either ON or OFF) when the button is short pressed
//Buzzer
Pin 17/GPIO 17 -> It activates the buzzzer and play a tone if the button is pressed for long time
//Button 
Pin 32/GPIO 32 -> Detect the button input whether it is long pressed or short pressed
//I2C PINS
Pin 21/SDA -> for Data  transfer and Pin22/SCL -> for Clock Cycle combine show OLED Display

Logic Summary 
When the button pressed is detected to be less than 2 seconds ,the LED toggles either ON or OFF 
When the button pressed is detected to be more  than 2 seconds ,the buzzer wil be activated and play a tone
The OLED displays the actions like what is happening either it is long press or short press if long press so it say long press Buzzer Activate 






