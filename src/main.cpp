#include <vector>

#include "../include/helpers.h"
#include "../include/io.h"
#include "../include/json.h"
#include "../include/keplerianApprox.h"
#include "../include/nBodyApprox.h"
#include "../include/picture.h"
#include "../include/planet.h"


int main() {
  // Initialize system
  std::vector<OrbitalElements> elements;
  std::vector<StateVector> bodies;
  populatePlanets(elements, bodies);

  // Initialize picture
  const size_t picSize = 500;
  const size_t systemSizeAU = approxSystemSize(elements);
  Picture pic(picSize, picSize, 0, 0, 0);

  const double daysSinceEpoch = getDate();
  keplerianApprox(elements, bodies, daysSinceEpoch);
  //   nBodyApprox(bodies, daysSinceEpoch);

  printTest(bodies, daysSinceEpoch);
  //   printResults(bodies);
  drawBodies(bodies, pic, systemSizeAU);
  pic.save("result.png");
}
