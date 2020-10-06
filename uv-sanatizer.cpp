// Sanitizer R H Fortenberry 10/4/2020

// Program to control UV-C germicidal lamps in disinfection chamber
// Single button interface with LED lighted push-button
// Upon powerup blink LED button 3 quick times to indicate box is initialized
// and ready to use
// After initialize LED is off and waiting in a forever while for Push to go
// low then high
// indicating the button is pressed and released
// When the LED is on, the UV is powered up
// While on the sanitize cycle can be interrupted by pressing the lighted
// button.  This will //instantly turn off the UV and lockout the start button
// for 5 seconds.  After the 5 seconds have //expired, the system will return to
// the initialized state waiting for another start signal
// The arduino is powered by a 5v usb source plugged into the single 110v
// yellow outlet
// on the bottom of the control box.  An USB A to mini USB cable provides
// power to the arduino //nano and also serves as the programming cable should
// you need to modify the code

#include <Arduino.h>

// this sets the number of seconds the UV tubes will be on for a sanitize cycle
const auto CookTime = 210;

// this goes high to power relay to turn on UV tubes
const auto UV = 2;

// this goes high to power LED
const auto LED = 3;

// set this low for the cathode of the LED
const auto LED_Cathode = 4;

// this is held high by pullup until button pressed then pulled low by pin 6
const auto Push = 5;

// this is the low side of the push button switch
const auto Push_Low = 6;

const auto flash_delay = 50;
const auto blink_count = 10;
const auto power_on_blink_count = 3;
const auto debounce_delay = 5;
const auto cancel_lockout_time = 5000;
const auto cook_sample_frequency = 10;
const auto cook_sample_delay = 1000 / 10;
const auto sanatize_delay = 1000;
const auto power_on_delay = 1000;

void lightOn() { digitalWrite(LED, HIGH); }

void lightOff() { digitalWrite(LED, LOW); }

void blink()
{
  for (int i = 0; i < blink_count; i++) {
    lightOn();
    delay(flash_delay);
    lightOff();
    delay(flash_delay);
  }
}

void powerOn()
{
  for (int i = 0; i < power_on_blink_count; i++) {
    lightOn();
    delay(flash_delay);
    lightOff();
    delay(flash_delay);
  }
}

void waitPress()
{
  while (digitalRead(Push) == HIGH) {}
  delay(debounce_delay);
  while (digitalRead(Push) == LOW) {}
}

void UVon() { digitalWrite(UV, HIGH); }

void UVoff() { digitalWrite(UV, LOW); }

int Sanitize()
{
  UVon();
  lightOn();
  delay(sanatize_delay);

  for (int i = 0; i < CookTime; i++) {
    for (int j = 0; j < cook_sample_frequency; j++) {
      if (digitalRead(Push) == LOW) {
        UVoff();
        lightOff();
        delay(cancel_lockout_time);
        blink();
        return 1;
      }

      delay(cook_sample_delay);
    }
  }

  UVoff();
  lightOff();
}

void setup()
{
  pinMode(UV, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED_Cathode, OUTPUT);
  pinMode(Push, INPUT_PULLUP);
  pinMode(Push_Low, OUTPUT);
  digitalWrite(UV, LOW);
  digitalWrite(UV, LOW);
  digitalWrite(LED_Cathode, LOW);
  digitalWrite(Push_Low, LOW);
  delay(power_on_delay);
  powerOn();
}

void loop()
{
  waitPress();
  Sanitize();
}
