#include <cmath>
#include <vector>

#include "../include/helpers.h"
#include "../include/io.h"
#include "../include/json.h"
#include "../include/keplerianApprox.h"
#include "../include/nBodyApprox.h"
#include "../include/picture.h"
#include "../include/planet.h"
#include "../include/timer.h"
#include "../include/util.h"


// N-body model of Jovian planets
void nBodyApprox(const std::vector<OrbitalElements> &elements,
                 std::vector<CelestialBody> &bodies, double julianDay);


// One-body approximation for Terrestrial planets
void keplerianApprox(const std::vector<OrbitalElements> &elements,
                     std::vector<CelestialBody> &bodies,
                     const double julianDay);


int main() {
  // Initialize system
  std::vector<OrbitalElements> elements;
  std::vector<CelestialBody> bodies;
  populatePlanets(elements, bodies);

  // Initialize picture
  const size_t picSize = 500;
  const size_t systemSizeAU = approxSystemSize(elements);
  Picture pic(picSize, picSize, 0, 0, 0);

  const double julianDay = getDate();

  // Update planets to target date
  //   keplerianApprox(elements, bodies, julianDay);
  nBodyApprox(elements, bodies, julianDay);

  // Output formatted results
  printTest(bodies, julianDay);
  //   printResults(bodies);

  drawBodies(bodies, pic, systemSizeAU, true);
  pic.save("result.png");
}


// N-body model of Jovian planets
void nBodyApprox(const std::vector<OrbitalElements> &elements,
                 std::vector<CelestialBody> &bodies, double julianDay) {

  // Use properties of Keplerian orbit to compute initial state vectors
  for (size_t i = 0; i < bodies.size() - 1; i++) {
    populateStateVectors(elements[i], bodies[i], 0);
  }

  // Numerically integrate, using each step to update planet
  const int dt = (julianDay < 0 ? -1 : 1) * SEC_PER_DAY / 4; // 6-hours
  const int steps = round(SEC_PER_DAY * abs(julianDay) / double(abs(dt)));
  for (int i = 0; i < steps; i++) {
    updateBodies(bodies, dt);
  }
};


// One-body approximation for Terrestrial planets
void keplerianApprox(const std::vector<OrbitalElements> &elements,
                     std::vector<CelestialBody> &bodies,
                     const double julianDay) {

  for (size_t i = 0; i < bodies.size() - 1; i++) {
    populateStateVectors(elements[i], bodies[i], julianDay);
  }
};
