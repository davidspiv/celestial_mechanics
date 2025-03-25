#include "../include/picture.h"
#include "../include/planet.h"
#include "../include/util.h"

#include <cmath>
#include <vector>

const rgbColor cPath = {61, 23, 193};
const rgbColor cSun = {255, 245, 160};
const rgbColor cMercury = {200, 200, 200};
const rgbColor cVenus = {240, 200, 170};
const rgbColor cEarth = {100, 180, 255};
const rgbColor cMars = {255, 110, 90};
const rgbColor cJupiter = {230, 190, 140};
const rgbColor cSaturn = {220, 200, 160};
const rgbColor cUranus = {180, 220, 220};
const rgbColor cNeptune = {90, 140, 200};

void drawBodies(const std::vector<StateVector> &bodies, Picture &pic,
                size_t systemSize, bool isPath = true) {

  const static int center = std::round(pic.width() / 2.0);

  auto drawPlanet = [&pic](const std::string &name, int x, int y) {
    rgbColor planetColor;
    if (name == "sun") {
      planetColor = cSun;
    } else if (name == "mercury") {
      planetColor = cMercury;
    } else if (name == "venus") {
      planetColor = cVenus;
    } else if (name == "earth") {
      planetColor = cEarth;
    } else if (name == "mars") {
      planetColor = cMars;
    } else if (name == "jupiter") {
      planetColor = cJupiter;
    } else if (name == "saturn") {
      planetColor = cSaturn;
    } else if (name == "uranus") {
      planetColor = cUranus;
    } else if (name == "neptune") {
      planetColor = cNeptune;
    }

    // Draw 9x9 square
    for (int i = -1; i <= 1; i++) {
      for (int j = -1; j <= 1; j++) {
        pic.set(x + i, y + j, planetColor);
      }
    }
  };

  for (auto &b : bodies) {
    Coord pos = b.pos / M_PER_AU;
    int x = scaleValue(pos.x, systemSize, center) + center;
    int y = scaleValue(-pos.y, systemSize, center) + center;

    if (isPath) {
      pic.set(x, y, cPath);
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
