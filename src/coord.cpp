#include <cmath>
#include <iomanip>
#include <iostream>

#include "../include/coord.h"

Coord::Coord() : x(0), y(0), z(0) {};

Coord::Coord(double x, double y, double z) : x(x), y(y), z(z) {};

double Coord::magSquared(const Coord &other) const {
  const double xD = other.x - x;
  const double yD = other.y - y;
  const double zD = other.z - z;
  return xD * xD + yD * yD + zD * zD;
}

void Coord::print() const {
  std::cout << std::left << std::fixed << std::setprecision(5)
            << "X: " << std::setw(14) << x << "Y: " << std::setw(14) << y
            << "Z: " << std::setw(14) << z << '\n'
            << std::right;
}

Coord Coord::operator+(const Coord &other) const {
  return {x + other.x, y + other.y, z + other.z};
}

Coord Coord::operator-(const Coord &other) const {
  return {x - other.x, y - other.y, z - other.z};
}


Coord Coord::operator*(const double scalar) const {
  return {x * scalar, y * scalar, z * scalar};
}

Coord Coord::operator*(const Coord &other) const {
  return {x * other.x, y * other.y, z * other.z};
}

Coord Coord::operator/(const Coord &other) const {
  return {x / other.x, y / other.y, z / other.z};
}


Coord Coord::operator/(double scalar) const {
  scalar = 1.0 / scalar;
  return {x * scalar, y * scalar, z * scalar};
}

Coord &Coord::operator+=(const Coord &other) {
  x += other.x;
  y += other.y;
  z += other.z;
  return *this;
}
