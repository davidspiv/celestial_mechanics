#ifndef UPDATE_H
#define UPDATE_H

#include <vector>

#include "picture.h"
#include "planet.h"

// N-body model of Jovian planets
void nBodyApprox(std::vector<StateVector> &bodies, double daysSinceEpoch,
                 Picture &pic, size_t systemSize);

#endif
