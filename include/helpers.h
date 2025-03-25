
#ifndef HELPER_H
#define HELPER_H

#include <iostream>
#include <vector>

#include "picture.h"
#include "planet.h"


void drawBodies(const std::vector<StateVector> &bodies, Picture &pic,
                size_t systemSize, bool isPath = true);

// approximates system size, assumes eccentricity is low
size_t approxSystemSize(const std::vector<OrbitalElements> &elements);


#endif
