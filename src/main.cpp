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
void nBodyApprox(std::vector<OrbitalStateVectors> &bodies,
                 double daysSinceEpoch);


// One-body approximation for Terrestrial planets
void keplerianApprox(const std::vector<OrbitalElements> &elements,
                     std::vector<OrbitalStateVectors> &bodies,
                     const double daysSinceEpoch);


int main() {
  // Initialize system
  std::vector<OrbitalElements> elements;
  std::vector<OrbitalStateVectors> bodies;
  populatePlanets(elements, bodies);

  // Initialize picture
  const size_t picSize = 500;
  const size_t systemSizeAU = approxSystemSize(elements);
  Picture pic(picSize, picSize, 0, 0, 0);

  const double daysSinceEpoch = getDate();

  // Update planets to target date
  //   keplerianApprox(elements, bodies, daysSinceEpoch);
  nBodyApprox(bodies, daysSinceEpoch);

  // Output formatted results
  printTest(bodies, daysSinceEpoch);
  //   printResults(bodies);

  drawBodies(bodies, pic, systemSizeAU, true);
  pic.save("result.png");
}


// N-body model of Jovian planets
void nBodyApprox(std::vector<OrbitalStateVectors> &bodies,
                 double daysSinceEpoch) {

  populateStateVectors(bodies);

  // Numerically integrate, using each step to update planet
  const int dt = (daysSinceEpoch < 0 ? -1 : 1) * SEC_PER_DAY / 4; // 6-hours
  const int steps = round(SEC_PER_DAY * abs(daysSinceEpoch) / double(abs(dt)));
  for (int i = 0; i < steps; i++) {
    updateBodies(bodies, dt);
  }
};


// One-body approximation for Terrestrial planets
void keplerianApprox(const std::vector<OrbitalElements> &elements,
                     std::vector<OrbitalStateVectors> &bodies,
                     const double daysSinceEpoch) {

  for (size_t i = 0; i < bodies.size() - 1; i++) {
    calcStateVectors(elements[i], bodies[i], daysSinceEpoch);
  }
};
