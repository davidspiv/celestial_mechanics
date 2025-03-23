
#ifndef HELPER_H
#define HELPER_H

#include <vector>

#include "picture.h"
#include "planet.h"

// N-body model of Jovian planets
void handleJovianBodies(const std::vector<OrbitalElements> &elements,
                        std::vector<CelestialBody> &bodies,
                        const double julianDay, const size_t planetDivideIndex);


// One-body approximation for Terrestrial planets
void handleTerrestrialBodies(const std::vector<OrbitalElements> &elements,
                             std::vector<CelestialBody> &bodies,
                             const double julianDay,
                             const size_t planetDivideIndex);


void drawBodies(const std::vector<CelestialBody> &planets, Picture &pic,
                size_t systemSize, bool finalPos);


#endif
