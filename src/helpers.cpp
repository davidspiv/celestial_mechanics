#include "../include/picture.h"
#include "../include/planet.h"
#include "../include/util.h"

#include <cmath>
#include <vector>

void drawBodies(const std::vector<StateVector> &bodies, Picture &pic,
                size_t systemSize, bool isPath = true) {

  const static int center = std::round(pic.width() / 2.0);

  auto drawPlanet = [&pic](const std::string &name, int x, int y) {
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        if (name == "sun") {
          pic.set(x + i, y + j, 255, 245, 160);
        } else if (name == "mercury") {
          pic.set(x + i, y + j, 200, 200, 200);
        } else if (name == "venus") {
          pic.set(x + i, y + j, 240, 200, 170);
        } else if (name == "earth") {
          pic.set(x + i, y + j, 100, 180, 255);
        } else if (name == "mars") {
          pic.set(x + i, y + j, 255, 110, 90);
        } else if (name == "jupiter") {
          pic.set(x + i, y + j, 230, 190, 140);
        } else if (name == "saturn") {
          pic.set(x + i, y + j, 220, 200, 160);
        } else if (name == "uranus") {
          pic.set(x + i, y + j, 180, 220, 220);
        } else if (name == "neptune") {
          pic.set(x + i, y + j, 90, 140, 200);
        }
      }
    }
  };

  for (auto &b : bodies) {
    Coord pos = b.pos / M_PER_AU;
    int x = scaleValue(pos.x, systemSize, center) + center;
    int y = scaleValue(-pos.y, systemSize, center) + center;

    if (isPath) {
      pic.set(x, y, 61, 23, 193);
    } else {
      drawPlanet(b.name, x, y);
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
