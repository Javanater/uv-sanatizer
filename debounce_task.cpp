#include "debounce_task.hpp"


debounce_task_t::debounce_task_t(long long debounce_time_)
  : debounce_time(debounce_time_)
{}

bool debounce_task_t::operator()(bool active, unsigned long current_time)
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