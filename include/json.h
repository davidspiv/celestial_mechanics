#ifndef JSON_H
#define JSON_H

#include <vector>

#include "planet.h"


// reads planets.json into a parallel vectors
void populatePlanets(std::vector<OrbitalElements> &elements,
                     std::vector<StateVector> &bodies);


void populateSolutions(std::vector<StateVector> &bodies,
                       const double daysSinceEpoch);

void populateStateVectors(std::vector<StateVector> &bodies);


#endif
