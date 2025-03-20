#ifndef COORD_H
#define COORD_H

struct Coord {

  Coord();
  Coord(double x, double y, double z);

  double magSquared(const Coord &other) const;

  void print() const;

  Coord operator+(const Coord &other) const;
  Coord operator*(const double scalar) const;
  Coord operator*(const Coord &other) const;
  Coord operator/(double scalar) const;
  Coord &operator+=(const Coord &other);
  Coord operator-(const Coord &other) const;
  Coord operator/(const Coord &other) const;

  double x, y, z;
};

#endif
