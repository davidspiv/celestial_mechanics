#ifndef PLANET_STATE_H
#define PLANET_STATE_H

#include <vector>

#include "coord.h"
#include "planet.h"

void keplerianApprox(const std::vector<OrbitalElements> &elements,
                     std::vector<StateVector> &bodies,
                     const double daysSinceEpoch);

#endif
