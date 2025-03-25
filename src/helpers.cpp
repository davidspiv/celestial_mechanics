#include "../include/picture.h"
#include "../include/planet.h"
#include "../include/util.h"

#include <cmath>
#include <vector>


void drawBodies(const std::vector<StateVector> &bodies, Picture &pic,
                size_t systemSize) {

  for (auto &b : bodies) {
    Coord pos = b.pos / M_PER_AU;
    int x = scaleValue(pos.x, systemSize, pic.width() / 2) + pic.width() / 2;
    int y = scaleValue(-pos.y, systemSize, pic.width() / 2) + pic.width() / 2;
    pic.set(x, y, 0, 255, 0);
  }
}


// approximates system size, assumes eccentricity is low
size_t approxSystemSize(const std::vector<OrbitalElements> &elements) {
  double systemSize = 0.0;
  for (auto p : elements) {
    if (p.semiMajorAxis > systemSize) {
      systemSize = p.semiMajorAxis;
    }
  }
  return size_t(std::ceil(systemSize / M_PER_AU));
}
