#ifndef __RISING_EDGE_TASK__
#define __RISING_EDGE_TASK__

struct rising_edge_task_t
{
  bool operator()(bool active);

private:
  bool last_active{ false };
};

#endif