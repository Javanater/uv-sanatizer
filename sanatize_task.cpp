#include "sanatize_task.hpp"

sanatize_task_t::sanatize_task_t(long long const sanatize_time_,
  long long const cool_down_time_)
  : sanatize_time(sanatize_time_), cool_down_time(cool_down_time_)
{}

bool sanatize_task_t::operator()(bool input, unsigned long current_time)
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
    if (input || current_time - sanatize_start_time >= sanatize_time) {
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
    if (current_time - sanatize_start_time >= cool_down_time) { state = IDLE; }
    break;
  }

  return state == SANATIZE;
}

sanatize_task_t::state_t sanatize_task_t::get_state() const { return state; }
