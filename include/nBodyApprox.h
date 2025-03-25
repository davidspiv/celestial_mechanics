#ifndef UPDATE_H
#define UPDATE_H

#include <vector>

#include "picture.h"
#include "planet.h"

void updateBodies(std::vector<OrbitalStateVectors> &planets, const int dt);

#endif
