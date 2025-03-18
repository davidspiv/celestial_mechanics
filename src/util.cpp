#include <math.h>

// converts degrees to a value between 0 and 360
double normalizeDegrees(double x) { return x - floor(x / 360.0) * 360.0; }

// converts degrees to radians
double toRadians(double degrees) { return degrees * (M_PI / 180.0); }
