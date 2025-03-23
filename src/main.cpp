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

  // N-body approximation for Jovian planets
  // Use properties of Keplerian orbit to compute initial state vectors
  for (size_t i = 0; i < bodies.size() - 1; i++) {
    populateStateVectors(elements[i], bodies[i], 0);
  }
  // Numerically integrate, using each step to update planet
  const int dt = SEC_PER_DAY * 7;
  const int steps = round(SEC_PER_DAY * julianDay / double(dt));
  for (int i = 0; i < steps; i++) {
    updateBodies(bodies, dt);
  }

  // One-body approximation using properties of Keplerian orbit
  const double planetDivideIndex = 5;
  for (size_t i = 0; i < planetDivideIndex; i++) {
    populateStateVectors(elements[i], bodies[i], julianDay);
  }

  // Handle photo
  const size_t picSize = 500;
  const size_t systemSizeAU = approxSystemSize(elements);
  Picture pic(picSize, picSize, 0, 0, 0);
  drawBodies(bodies, pic, systemSizeAU, true);
  pic.save("result.png");

  // Output formatted results
  //   printTest(bodies, julianDay);
  printResults(bodies);
}
