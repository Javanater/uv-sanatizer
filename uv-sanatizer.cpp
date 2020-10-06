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

struct debounce_task_t
{
  enum state_t { INACTIVE, DEBOUCE_INACTIVE, ACTIVE, DEBOUCE_ACTIVE };
  using active_t = decltype(HIGH);
  using mode_t = decltype(INPUT_PULLUP);

  explicit debounce_task_t(long long debounce_time_)
    : debounce_time(debounce_time_)
  {}

  bool operator()(bool active, unsigned long current_time)
  {
    switch (state) {
    case INACTIVE:
      if (active) {
        last_transition_time = current_time;
        state = DEBOUCE_INACTIVE;
      }
      break;

    case DEBOUCE_INACTIVE:
      if (!active) {
        state = INACTIVE;
      } else if (current_time - last_transition_time >= debounce_time) {
        state = ACTIVE;
      }
      break;

    case ACTIVE:
      if (!active) {
        last_transition_time = current_time;
        state = DEBOUCE_ACTIVE;
      }
      break;

    case DEBOUCE_ACTIVE:
      if (active) {
        state = ACTIVE;
      } else if (current_time - last_transition_time >= debounce_time) {
        state = INACTIVE;
      }
      break;
    }

    return state == ACTIVE || state == DEBOUCE_ACTIVE;
  }

private:
  state_t state{ INACTIVE };
  long long const debounce_time;
  long long last_transition_time{ 0 };
};

struct sanatize_task_t
{
  enum state_t {
    IDLE,
    IDLE_WAIT_FOR_RELEASE,
    SANATIZE,
    CANCEL_WAIT_FOR_RELEASE,
    COOL_DOWN
  };

  sanatize_task_t(long long const sanatize_time_,
    long long const cool_down_time_)
    : sanatize_time(sanatize_time_), cool_down_time(cool_down_time_)
  {}

  bool operator()(bool input, unsigned long current_time)
  {
    switch (state) {
    case IDLE:
      if (input) { state = IDLE_WAIT_FOR_RELEASE; }
      break;

    case IDLE_WAIT_FOR_RELEASE:
      if (!input) {
        state = SANATIZE;
        sanatize_start_time = current_time;
      }
      break;

    case SANATIZE:
      if (input) {
        state = CANCEL_WAIT_FOR_RELEASE;
      } else if (current_time - sanatize_start_time >= sanatize_time) {
        state = CANCEL_WAIT_FOR_RELEASE;
      }
      break;

    case CANCEL_WAIT_FOR_RELEASE:
      if (!input) {
        state = COOL_DOWN;
        sanatize_start_time = current_time;
      }
      break;

    case COOL_DOWN:
      if (current_time - sanatize_start_time >= cool_down_time) {
        state = IDLE;
      }
      break;
    }

    return state == SANATIZE;
  }

private:
  state_t state{ IDLE };
  long long const sanatize_time;
  long long const cool_down_time;
  long long sanatize_start_time{ 0 };
};

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
