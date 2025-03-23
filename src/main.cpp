#include <cmath>
#include <vector>

#include "../include/getInitialPlanetState.h"
#include "../include/helpers.h"
#include "../include/io.h"
#include "../include/picture.h"
#include "../include/planet.h"
#include "../include/timer.h"


int main() {
  // Parallel vectors to represent planets
  std::vector<OrbitalElements> elements;
  std::vector<CelestialBody> bodies;

  // Initialize planets
  const double julianDay = getDate();
  const size_t planetDivideIndex = 4;
  populatePlanets(elements, bodies);

  // Update planets to target date
  handleJovianBodies(elements, bodies, julianDay, planetDivideIndex);
  handleTerrestrialBodies(elements, bodies, julianDay, planetDivideIndex);

  // Handle PNG output
  const size_t picSize = 500;
  const size_t systemSizeAU = approxSystemSize(elements);
  Picture pic(picSize, picSize, 0, 0, 0);
  drawBodies(bodies, pic, systemSizeAU, true);
  pic.save("result.png");

  // Output formatted results
  //   printTest(bodies, julianDay);
  printResults(bodies);
}
