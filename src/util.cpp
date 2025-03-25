#include <cmath>
#include <iostream>

// converts degrees to a value between 0 and 360
double normalizeRadians(const double x) {
  const double twoPi = 2.0 * M_PI;
  return x - floor(x / twoPi) * twoPi;
}


// converts degrees to radians
double toRadians(const double degrees) { return degrees * (M_PI / 180.0); }


// Scales a value from a current maximum range to a new maximum range
int scaleValue(const double x, const int currMax, const int newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}
