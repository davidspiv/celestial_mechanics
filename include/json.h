#ifndef JSON_H
#define JSON_H

#include <vector>

#include "planet.h"


// reads planets.json into a parallel vectors
void populatePlanets(std::vector<OrbitalElements> &elements,
                     std::vector<CelestialBody> &bodies);


void populateSolutions(std::vector<CelestialBody> &bodies,
                       const double julianDay);


#endif
