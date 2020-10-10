#ifndef __SANATIZE_TASK_HPP__
#define __SANATIZE_TASK_HPP__

#include "ints.hpp"

struct sanatize_task_t
{
  enum state_t { IDLE, SANATIZE, COOL_DOWN };

  sanatize_task_t(duration_t sanatize_time_, duration_t cool_down_time_);

  bool operator()(bool input, timepoint_t current_time);

  state_t get_state() const;

private:
  state_t state{ IDLE };
  duration_t const sanatize_time;
  duration_t const cool_down_time;
  timepoint_t sanatize_start_time{ 0 };
};

char const *to_string(sanatize_task_t::state_t state);
#endif