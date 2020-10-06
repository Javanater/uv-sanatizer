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
#include "debounce_task.hpp"
#include "sanatize_task.hpp"

// this sets the number of seconds the UV tubes will be on for a sanitize cycle
const auto CookTime = 210000;

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

debounce_task_t debounce{ debounce_delay };
sanatize_task_t sanatize{ CookTime, cancel_lockout_time };

void setup()
{
  pinMode(UV, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED_Cathode, OUTPUT);
  pinMode(Push, INPUT_PULLUP);
  pinMode(Push_Low, OUTPUT);
  digitalWrite(UV, LOW);
  digitalWrite(LED_Cathode, LOW);
  digitalWrite(Push_Low, LOW);
}

bool read_button() { return digitalRead(Push) == HIGH; }

void set_uv(bool active) { digitalWrite(UV, active ? HIGH : LOW); }

void loop()
{
  auto current_time = millis();
  bool button_state = read_button();
  bool debounced_button_state = debounce(button_state, current_time);
  bool uv_active = sanatize(debounced_button_state, current_time);
  set_uv(uv_active);
}
