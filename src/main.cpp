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
  // Parallel vectors to represent planets
  std::vector<OrbitalElements> elements;
  std::vector<CelestialBody> bodies;

  // Initialize planets
  const double julianDay = getDate();
  populatePlanets(elements, bodies);
  for (size_t i = 0; i < bodies.size() - 1; i++) {
    populateStateVectors(elements[i], bodies[i], 0);
  }

  // Initialize photo
  const size_t picSize = 500;
  const size_t systemSizeAU = approxSystemSize(elements);
  Picture pic(picSize, picSize, 0, 0, 0);

  // Update planets and photo
  const int dt = SEC_PER_DAY; // 1-week intervals
  const int steps = round(SEC_PER_DAY * julianDay / double(dt));
  for (int i = 0; i < steps; i++) {
    updateBodies(bodies, dt);
    drawBodies(bodies, pic, systemSizeAU);
  }

  // Handle results
  drawBodies(bodies, pic, systemSizeAU, true);
  pic.save("result.png");
  //   printTest(bodies, julianDay);
  printResults(bodies);
}
