// Sanitizer R H Fortenberry 10/4/2020
// // Program to control UV-C germicidal lamps in disinfection chamber
// // Single button interface with LED lighted push-button
// // Upon powerup blink LED button 3 quick times to indicate box is initialized and ready to use
// // After initialize LED is off and waiting in a forever while for Push to go low then high
// // indicating the button is pressed and released
// // When the LED is on, the UV is powered up
// // While on the sanitize cycle can be interrupted by pressing the lighted button.  This will //instantly turn off the UV and lockout the start button for 5 seconds.  After the 5 seconds have //expired, the system will return to the initialized state waiting for another start signal   
// //The arduino is powered by a 5v usb source plugged into the single 110v yellow outlet
// //on the bottom of the control box.  An USB A to mini USB cable provides power to the arduino //nano and also serves as the programming cable should you need to modify the code

#include <Arduino.h>

#define CookTime 210  // this sets the number of seconds the UV tubes will be on for a sanitize cycle
#define UV 2  // this goes high to power relay to turn on UV tubes
#define LED 3  //this goes high to power LED
// LED Cathode 4  set this low for the cathode of the LED
#define Push 5  // this is held high by pullup until button pressed then pulled low by pin 6
// Push Low 6   // this is the low side of the push button switch

void lightOn()
{
  digitalWrite (LED, HIGH);
}

void lightOff()
{
  digitalWrite (LED, LOW);
}  

void blink()
{
  for( int i=0;i < 10; i++)
  {
    lightOn();
    delay(50);
    lightOff();
    delay(50);
  }
}

void powerOn()
{
  for( int i=0;i < 3; i++)
  {
    lightOn();
    delay(50);
    lightOff();
    delay(50);
  }
}

void waitPress()
{
  while (digitalRead(Push) == HIGH){}
  delay(5);
  while (digitalRead(Push) == LOW){}
}

void UVon()
{
  digitalWrite(UV , HIGH);
}

void UVoff()
{
  digitalWrite(UV, LOW);
}

int Sanitize()
{
  UVon();
  lightOn();
  delay (1000);
  
  for (int i=0; i < CookTime; i++)
  {
    for (int j=0;j< 10;j++)
    {
      if (digitalRead(Push) == LOW)
      {
        UVoff();
        lightOff();
        delay(5000);
        blink();
        return(1);
      }
      
      delay(100);
    }
  }  
  
  UVoff();
  lightOff();
}

void setup() {
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(Push,INPUT_PULLUP);
  pinMode(6,OUTPUT);
  digitalWrite(2, LOW);
  digitalWrite(UV, LOW);
  digitalWrite( 4, LOW);
  digitalWrite (6, LOW);
  delay(1000);
  powerOn();
}

void loop()
{
  waitPress();
  Sanitize();
}
