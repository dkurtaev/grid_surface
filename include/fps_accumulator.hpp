#ifndef INCLUDE_FPS_ACCUMULATOR_HPP_
#define INCLUDE_FPS_ACCUMULATOR_HPP_

#include <sys/time.h>

#include <queue>

class FpsAccumulator {
 public:
  void Accumulate() {
    // Add frame time.
    timeval current_time;
    gettimeofday(&current_time, 0);
    frames_times.push(current_time);

    // Remove old times.
    do {
      if (TimeFrom(frames_times.front()) > kPeriod) {
        frames_times.pop();
      } else {
        break;
      }
    } while (!frames_times.empty());
  }

  int GetFps() {
    return frames_times.size() * (1000.0f / kPeriod);
  }

  float TimeFrom(const timeval& tv) {
    timeval current_tv;
    gettimeofday(&current_tv, 0);
    return ((current_tv.tv_sec - tv.tv_sec) * 1e+3 +
            (current_tv.tv_usec - tv.tv_usec) * 1e-3);
  }

 private:
  // Time in milliseconds when collects frames times for computing mean fps.
  static const unsigned kPeriod = 1000;
  std::queue<timeval> frames_times;
};

#endif  // INCLUDE_FPS_ACCUMULATOR_HPP_
