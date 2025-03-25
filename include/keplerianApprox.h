#ifndef PLANET_STATE_H
#define PLANET_STATE_H

#include <vector>

#include "coord.h"
#include "planet.h"

// calculates heliocentric position and velocity vectors
void calcStateVectors(const OrbitalElements &element, OrbitalStateVectors &body,
                          float daysSinceEpoch);

#endif
