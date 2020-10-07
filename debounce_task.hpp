#ifndef __DEBOUNCE_TASK_HPP__
#define __DEBOUNCE_TASK_HPP__

struct debounce_task_t
{
  enum state_t { INACTIVE, DEBOUCE_INACTIVE, ACTIVE, DEBOUCE_ACTIVE };

  explicit debounce_task_t(long long debounce_time_);

  bool operator()(bool active, unsigned long current_time);

  state_t get_state() const;

private:
  state_t state{ INACTIVE };
  long long const debounce_time;
  long long last_transition_time{ 0 };
};

#endif