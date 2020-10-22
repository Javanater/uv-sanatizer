#include "sanatize_task.hpp"

sanatize_task_t::sanatize_task_t(duration_t const sanatize_time_,
  duration_t const cool_down_time_)
  : sanatize_time(sanatize_time_), cool_down_time(cool_down_time_)
{}

bool sanatize_task_t::operator()(bool input, timepoint_t current_time)
{
  switch (state) {
  case IDLE:
    if (input) {
      state = SANATIZE;
      sanatize_start_time = current_time;
    }
    break;

  case SANATIZE:
    if (input || current_time - sanatize_start_time >= sanatize_time) {
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

// cppcheck-suppress unusedFunction symbolName=get_state
sanatize_task_t::state_t sanatize_task_t::get_state() const { return state; }

// cppcheck-suppress unusedFunction symbolName=to_string
char const *to_string(sanatize_task_t::state_t state)
{
  switch (state) {
  case sanatize_task_t::IDLE:
    return "IDLE";

  case sanatize_task_t::SANATIZE:
    return "SANATIZE";

  case sanatize_task_t::COOL_DOWN:
    return "COOL_DOWN";
  }
}
