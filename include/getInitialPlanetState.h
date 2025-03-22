#ifndef PLANET_STATE_H
#define PLANET_STATE_H
#include "celestialBody.h"
#include "coord.h"

void populatePlanets(std::vector<OrbitalElements> &planets,
                     std::vector<CelestialBody> &bodies);

void populateStateVectors(const OrbitalElements &planet, CelestialBody &body);


size_t approxSystemSize(const std::vector<OrbitalElements> &planets);

#endif
