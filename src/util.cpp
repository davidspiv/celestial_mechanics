#include <cmath>
#include <iostream>

// converts degrees to a value between 0 and 360
double normalizeDegrees(double x) { return x - floor(x / 360.0) * 360.0; }


// converts degrees to radians
double toRadians(double degrees) { return degrees * (M_PI / 180.0); }


// Scales a value from a current maximum range to a new maximum range
size_t scaleValue(double x, size_t currMax, size_t newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}
