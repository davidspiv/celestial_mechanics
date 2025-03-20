#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>

#define M_PER_AU 149597870691
#define KM_PER_AU 1.496e8
#define M_PER_KM 1000
#define SEC_PER_DAY 86400
#define SEC_PER_YR 31556952

#define G 6.67430e-11   // Gravitational constant
#define M_SUN 1.9891e30 // [kg]

double normalizeDegrees(double x);

double toRadians(double degrees);

size_t scaleValue(double x, size_t currMax, size_t newMax);

#endif
