#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include "coord.h"
#include <string>

struct CelestialBody {
  std::string name;
  Coord pos;
  Coord vel;
  double mass;

  double semiMajorAxis;
  double eccentricity;
  double orbitalInclination;
  double longitudeOfAscendingNode;
  double longitudeOfPerihelion;
  double meanAnomaly;
  double period;
};

#endif
