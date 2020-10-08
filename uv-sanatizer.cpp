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
const auto debounce_delay = 100;
const auto cancel_lockout_time = 5000;
const auto cook_sample_frequency = 10;
const auto cook_sample_delay = 1000 / 10;
const auto sanatize_delay = 1000;
const auto power_on_delay = 1000;

debounce_task_t debounce{ debounce_delay };
rising_edge_task_t rising_edge;
sanatize_task_t sanatize{ CookTime, cancel_lockout_time };

bool read_button() { return digitalRead(Push) == LOW; }

bool check_initial_conditions() { return read_button() == false; }

void set_uv(bool active) { digitalWrite(UV, active ? HIGH : LOW); }

void set_led(bool active) { digitalWrite(LED, active ? HIGH : LOW); }

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
#ifdef SerialDebug
  SerialDebug.begin(115200);
  println("Boot up");
#endif

  pinMode(UV, OUTPUT);
  pinMode(LED, OUTPUT);
  pinMode(LED_Cathode, OUTPUT);
  pinMode(Push, INPUT_PULLUP);
  pinMode(Push_Low, OUTPUT);
  digitalWrite(UV, LOW);
  digitalWrite(LED_Cathode, LOW);
  digitalWrite(Push_Low, LOW);

  if (!check_initial_conditions()) {
#ifdef SerialDebug
    println(
      "ERROR: Initial conditions not met. Please check wiring and reboot.");
#endif
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

#ifdef SerialDebug
  auto static button_state_w = make_watcher("button_state", button_state);
  button_state_w(button_state);
  auto static debounced_button_state_w =
    make_watcher("debounced_button_state", debounced_button_state);
  debounced_button_state_w(debounced_button_state);
  auto static rising_edge_state_w =
    make_watcher("rising_edge_state", rising_edge_state);
  rising_edge_state_w(rising_edge_state);
  auto static uv_active_w = make_watcher("uv_active", uv_active);
  uv_active_w(uv_active);
  auto static sanatize_state_w =
    make_watcher("sanatize_state", sanatize.get_state());
  sanatize_state_w(sanatize.get_state());
#endif
}
