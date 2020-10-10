#ifndef __DEBUGGING_HPP__
#define __DEBUGGING_HPP__

#ifdef SerialDebug
inline void debug_init() { SerialDebug.begin(115200); }

template<class T> void println(T t) { SerialDebug.println(t); }

template<class T> struct watcher
{
  watcher(char const *const name_, T init) : name(name_), last_value(init)
  {
    print(init);
  }

  void operator()(T current_value)
  {
    if (last_value != current_value) {
      print(current_value);
      last_value = current_value;
    }
  }

  void print(T current_value)
  {
    SerialDebug.print(name);
    SerialDebug.print("=");
    println(current_value);
  }

private:
  T last_value;
  char const *const name;
};

template<class T> watcher<T> make_watcher(char const *const name, T init)
{
  return watcher<T>(name, init);
}

#define WATCH(x)                               \
  {                                            \
    auto static watcher = make_watcher(#x, x); \
    watcher(x);                                \
  }

#else

inline void debug_init() {}

template<class T> void println(T t) {}

#define WATCH(x) ((void)0);

#endif

#endif