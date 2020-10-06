#ifndef __SANATIZE_TASK_HPP__
#define __SANATIZE_TASK_HPP__

struct sanatize_task_t
{
  enum state_t {
    IDLE,
    IDLE_WAIT_FOR_RELEASE,
    SANATIZE,
    CANCEL_WAIT_FOR_RELEASE,
    COOL_DOWN
  };

  sanatize_task_t(long long sanatize_time_, long long cool_down_time_);

  bool operator()(bool input, unsigned long current_time);

private:
  state_t state{ IDLE };
  long long const sanatize_time;
  long long const cool_down_time;
  long long sanatize_start_time{ 0 };
};

#endif