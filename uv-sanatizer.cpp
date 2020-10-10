// Sanitizer R H Fortenberry 10/4/2020

// Program to control UV-C germicidal lamps in disinfection chamber
// Single button interface with LED lighted push-button
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
#include "rising_edge_task.hpp"
#include "sanatize_task.hpp"

#define SerialDebug Serial
#include "debugging.hpp"

#ifdef SerialDebug
template<> void println<sanatize_task_t::state_t>(sanatize_task_t::state_t t)
{
  SerialDebug.println(to_string(t));
}
#endif

// this sets the number of seconds the UV tubes will be on for a sanitize cycle
const auto cook_time = 210000;

// this goes high to power relay to turn on UV tubes
const auto uv_pin = 2;

// this goes high to power LED
const auto led_pin = 3;

// set this low for the cathode of the LED
const auto led_cathode = 4;

// this is held high by pullup until button pressed then pulled low by pin 6
const auto button_pin = 5;

// this is the low side of the push button switch
const auto button_gnd = 6;

const auto flash_delay = 50;
const auto debounce_delay = 50;
const auto cancel_lockout_time = 5000;

debounce_task_t debounce{ debounce_delay };
rising_edge_task_t rising_edge;
sanatize_task_t sanatize{ cook_time, cancel_lockout_time };

bool read_button() { return digitalRead(button_pin) == LOW; }

bool check_initial_conditions() { return read_button() == false; }

void set_uv(bool active) { digitalWrite(uv_pin, active ? HIGH : LOW); }

void set_led(bool active) { digitalWrite(led_pin, active ? HIGH : LOW); }

void error_state()
{
  for (;;) {
    set_led(true);
    delay(flash_delay);
    set_led(false);
    delay(flash_delay);
  }
}

void setup()
{
  debug_init();
  println("Boot up");

  pinMode(uv_pin, OUTPUT);
  pinMode(led_pin, OUTPUT);
  pinMode(led_cathode, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);
  pinMode(button_gnd, OUTPUT);
  digitalWrite(uv_pin, LOW);
  digitalWrite(led_cathode, LOW);
  digitalWrite(button_gnd, LOW);

  if (!check_initial_conditions()) {
    println(
      "ERROR: Initial conditions not met. Please check wiring and reboot.");
    error_state();
  }
}

void loop()
{
  auto current_time = millis();
  bool button_state = read_button();
  bool debounced_button_state = debounce(button_state, current_time);
  bool rising_edge_state = rising_edge(debounced_button_state);
  bool uv_active = sanatize(rising_edge_state, current_time);
  set_uv(uv_active);
  set_led(uv_active);

  WATCH(button_state);
  WATCH(debounced_button_state);
  WATCH(rising_edge_state);
  WATCH(uv_active);
  WATCH(sanatize.get_state());
}
