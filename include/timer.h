// https://www.youtube.com/watch?v=YG4jexlSAjc&t=711s

#include <chrono>
#include <iostream>

using namespace std;

class Timer {
private:
  chrono::time_point<chrono::high_resolution_clock> m_StartTimepoint =
      chrono::high_resolution_clock::now();

public:
  Timer() {};
  ~Timer() { Stop(); };

  void Stop() {
    auto endTimepoint = chrono::high_resolution_clock::now();

    auto start = chrono::time_point_cast<chrono::microseconds>(m_StartTimepoint)
                     .time_since_epoch()
                     .count();
    auto end = chrono::time_point_cast<chrono::microseconds>(endTimepoint)
                   .time_since_epoch()
                   .count();

    auto duration = end - start;
    double ms = duration * 0.001;
    cout << "duration (ms): " << ms << endl;
  }
};
