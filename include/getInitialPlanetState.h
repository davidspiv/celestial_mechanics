#ifndef PLANET_STATE_H
#define PLANET_STATE_H
#include "celestialBody.h"
#include "coord.h"


// reads planets.json into a parallel vectors
void populatePlanets(std::vector<OrbitalElements> &elements,
                     std::vector<CelestialBody> &bodies);


// calculates heliocentric position and velocity vectors
void populateStateVectors(const OrbitalElements &element, CelestialBody &body);


// approximates system size, assumes eccentricity is low
size_t approxSystemSize(const std::vector<OrbitalElements> &elements);

#endif
