#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include <string>

#define M_PER_AU 149597870691
#define KM_PER_AU 1.496e8
#define M_PER_KM 1000
#define SEC_PER_DAY 86400
#define SEC_PER_YR 31556952

#define JD_EPOCH 2451544.5

#define G 6.67430e-11   // Gravitational constant
#define M_SUN 1.9891e30 // [kg]

double normalizeRadians(const double x);

double toRadians(const double degrees);

int scaleValue(const double x, const int currMax, const int newMax);

#endif
