#include "../include/picture.h"
#include "../include/planet.h"
#include "../include/util.h"

#include <cmath>
#include <vector>


void drawBodies(const std::vector<OrbitalStateVectors> &planets, Picture &pic,
                size_t systemSize, bool finalPos) {

  for (OrbitalStateVectors p : planets) {
    Coord pos = p.pos / M_PER_AU;
    int x = scaleValue(pos.x, systemSize, pic.width() / 2) + pic.width() / 2;
    int y = scaleValue(-pos.y, systemSize, pic.width() / 2) + pic.width() / 2;
    if (p.name == "sun")
      pic.set(x, y, 255, 255, 0);
    else {
      if (!finalPos) {
        pic.set(x, y, 0, 255, 0);
      } else {
        pic.set(x, y, 255, 0, 0);
      }
    }
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
