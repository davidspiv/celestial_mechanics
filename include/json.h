#ifndef JSON_H
#define JSON_H

#include <vector>

#include "planet.h"


// reads planets.json into a parallel vectors
void populatePlanets(std::vector<OrbitalElements> &elements,
                     std::vector<OrbitalStateVectors> &bodies);


void populateSolutions(std::vector<OrbitalStateVectors> &bodies,
                       const double daysSinceEpoch);

void populateStateVectors(std::vector<OrbitalStateVectors> &bodies);


#endif
