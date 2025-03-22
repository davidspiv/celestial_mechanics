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
  //   const double julianDay = 366;
  Timer timer;
  const int dt = 600; // 10-minute intervals
  const int steps = round(SEC_PER_DAY * julianDay / double(dt));
  vector<CelestialBody> planets = populatePlanets();

  const int picSideLength = 500;
  const int picCenter = picSideLength / 2;
  Picture pic(picSideLength, picSideLength, 0, 0, 0);

  for (int i = 0; i < steps; i++) {
    planets = updateBodies(planets, dt);

    // end up rendering a lot of the same pixels, but checking is slower?
    drawBodies(planets, pic, picCenter);
  }
  drawBodies(planets, pic, picCenter, true);

  pic.save("result.png");
  printResults(planets);
}
