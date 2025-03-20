#ifndef UPDATE_H
#define UPDATE_H

#include <vector>

#include "celestialBody.h"

// Approximate new state vectors for a given interval using 4th-Order
// Runge-Kutta. Returns updated Celestial Body
CelestialBody rungeKuttaStep(const CelestialBody &p,
                             const std::vector<CelestialBody> &planets, int dt);

#endif
