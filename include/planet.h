#ifndef PLANET_H
#define PLANET_H

#include "coord.h"
#include <string>

struct OrbitalStateVectors {
  std::string name;
  Coord pos;
  Coord vel;
  double mass;
  double period;
};

struct OrbitalElements {
  double semiMajorAxis;
  double eccentricity;
  double orbitalInclination;
  double longitudeOfAscendingNode;
  double longitudeOfPerihelion;
  double meanAnomaly;
};

#endif
