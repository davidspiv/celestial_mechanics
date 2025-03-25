#include <vector>

#include "../include/helpers.h"
#include "../include/io.h"
#include "../include/json.h"
#include "../include/keplerianApprox.h"
#include "../include/nBodyApprox.h"
#include "../include/picture.h"
#include "../include/planet.h"
#include "../include/util.h"


int main() {
  // Initialize system
  std::vector<OrbitalElements> elements;
  std::vector<StateVector> bodies;
  populatePlanets(elements, bodies);
  static StateVector sun = {"sun", Coord(), Coord(), M_SUN};
  bodies.emplace_back(sun);

  // Initialize picture
  const size_t picSize = 2000;
  const size_t systemSize = approxSystemSize(elements);
  Picture pic(picSize, picSize, 13, 5, 41);

  const double daysSinceEpoch = getDate();
  //   keplerianApprox(elements, bodies, daysSinceEpoch);
  nBodyApprox(bodies, daysSinceEpoch, pic, systemSize);

  printTest(bodies, daysSinceEpoch);
  //   printResults(bodies);
  drawBodies(bodies, pic, systemSize, false);
  pic.save("result.png");
}
