#include <cmath>
#include <vector>

#include "../include/celestialBody.h"
#include "../include/coord.h"
#include "../include/getInitialPlanetState.h"
#include "../include/io.h"
#include "../include/picture.h"
#include "../include/timer.h"
#include "../include/updatePlanetState.h"
#include "../include/util.h"


int main() {
  const double julianDay = getDate();
  //   const double julianDay = 0;
  const int dt = 345600; // 1-week intervals
  const int steps = round(SEC_PER_DAY * julianDay / double(dt));

  const int picSideLength = 500;
  Picture pic(picSideLength, picSideLength, 0, 0, 0);

  // parallel vectors to represent planets
  std::vector<OrbitalElements> elements;
  std::vector<CelestialBody> bodies;

  //   populateSolutions(bodies, julianDay);
  //   const size_t systemSize = 31;
  populatePlanets(elements, bodies);
  const size_t systemSize = approxSystemSize(elements);

  for (size_t i = 0; i < bodies.size() - 1; i++) {
    populateStateVectors(elements[i], bodies[i], 0);
  }

  for (int i = 0; i < steps; i++) {
    updateBodies(bodies, dt);
    drawBodies(bodies, pic, systemSize);
  }
  drawBodies(bodies, pic, systemSize, true);

  pic.save("result.png");

  printTest(bodies, julianDay);
  //   printResults(bodies);
}
