#ifndef PLANET_STATE_H
#define PLANET_STATE_H

#include <vector>

#include "coord.h"
#include "planet.h"

// calculates heliocentric position and velocity vectors
void populateStateVectors(const OrbitalElements &element, CelestialBody &body,
                          float daysSinceEpoch);

#endif
