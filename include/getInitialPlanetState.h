#ifndef PLANET_STATE_H
#define PLANET_STATE_H
#include "celestialBody.h"
#include "coord.h"

std::vector<CelestialBody> populatePlanets();

void getInitialPlanetState(CelestialBody &planet);

#endif
