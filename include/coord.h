#include <cmath>
#include <iomanip>
#include <iostream>

class Coord {
public:
  Coord() : x(0), y(0), z(0) {};
  Coord(double x, double y, double z) : x(x), y(y), z(z) {};

  Coord rCoord(const Coord &other) const;
  double distSquared(const Coord &other) const;
  // magnitude()
  // normalize()

  void print() const;

  Coord operator+(const Coord &other) const;
  Coord operator*(const double scalar) const;
  Coord operator*(const Coord &other) const;
  Coord operator/(double scalar) const;
  Coord &operator+=(const Coord &other);
  Coord operator-(const Coord &other) const;
  Coord operator/(const Coord &other) const;
  // operator==
  // operator!=
  // operator-
  // operator-=

  //  private:
  double x, y, z;
};

Coord Coord::rCoord(const Coord &other) const {
  const double xD = other.x - x;
  const double yD = other.y - y;
  const double zD = other.z - z;
  return Coord(xD, yD, zD);
};

double Coord::distSquared(const Coord &other) const {
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
