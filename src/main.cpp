#include <algorithm>
#include <cmath>
#include <vector>

#include "../include/celestialBody.h"
#include "../include/coord.h"
#include "../include/date.h"
#include "../include/getInitialPlanetState.h"
#include "../include/io.h"
#include "../include/picture.h"
#include "../include/updatePlanetState.h"
#include "../include/util.h"

int main() {
  const double julianDay = getDate();
  const int dt = 600; // 10-minute intervals
  const int steps = round(SEC_PER_DAY * julianDay / double(dt));
  vector<CelestialBody> planets = populatePlanets();

  const int picSideLength = 500;
  const int picCenter = picSideLength / 2;
  Picture pic(picSideLength, picSideLength, 0, 0, 0);

  pic.set(picCenter, picCenter, 255, 255, 0);

  for (int i = 0; i < steps; i++) {
    std::vector<CelestialBody> updatedBodies;
    std::transform(planets.begin(), planets.end(),
                   std::back_inserter(updatedBodies),
                   [planets, dt](const CelestialBody &p) {
                     return rungeKuttaStep(p, planets, dt);
                   });
    planets = updatedBodies;

    for (CelestialBody p : planets) {
      Coord pos = p.pos / M_PER_AU;
      int x = scaleValue(pos.x, 31, picCenter) + picCenter;
      int y = scaleValue(-pos.y, 31, picCenter) + picCenter;
      pic.set(x, y, 0, 255, 0);
    }
  }

  printResults(planets);
}
