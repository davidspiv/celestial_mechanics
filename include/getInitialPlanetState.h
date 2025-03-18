#ifndef PLANET_STATE_H
#define PLANET_STATE_H
#include "celestialBody.h"
#include "coord.h"

void getPeriod(CelestialBody &p);

// Calculate mass from period (if needed)
void getMass(CelestialBody &p);

std::vector<CelestialBody> populatePlanets();

void getInitialPlanetState(CelestialBody &planet);

#endif
