#include "rising_edge_task.hpp"

bool rising_edge_task_t::operator()(bool active)
{
  if (active && !last_active) {
    last_active = active;
    return true;
  }

  last_active = active;
}