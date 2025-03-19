#ifndef COORD_H
#define COORD_H

#include <cmath>
#include <iomanip>
#include <iostream>

class Coord {
public:
  Coord();
  Coord(double x, double y, double z);

  Coord rCoord(const Coord &other) const;
  double magSquared(const Coord &other) const;
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

#endif
