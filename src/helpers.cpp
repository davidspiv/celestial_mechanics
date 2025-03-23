#include "../include/getInitialPlanetState.h"
#include "../include/planet.h"
#include "../include/updatePlanetState.h"
#include "../include/util.h"

#include <cmath>
#include <vector>


// N-body model of Jovian planets
void handleJovianBodies(const std::vector<OrbitalElements> &elements,
                        std::vector<CelestialBody> &bodies,
                        const double julianDay,
                        const size_t planetDivideIndex) {
  // Use properties of Keplerian orbit to compute initial state vectors
  for (size_t i = planetDivideIndex; i < bodies.size() - 1; i++) {
    populateStateVectors(elements[i], bodies[i], 0);
  }
  // Isolate bodies to include in model
  std::vector<CelestialBody> jovianBodies(bodies.begin() + planetDivideIndex,
                                          bodies.end());
  // Numerically integrate, using each step to update planet
  const int dt = SEC_PER_DAY * 7; // one week timestep
  const int steps = round(SEC_PER_DAY * julianDay / double(dt));
  for (int i = 0; i < steps; i++) {
    updateBodies(jovianBodies, dt);
  }
  // Merge back updated bodies
  for (size_t i = 0; i < jovianBodies.size(); i++) {
    bodies[i + planetDivideIndex] = jovianBodies[i];
  }
};


// One-body approximation for Terrestrial planets
void handleTerrestrialBodies(const std::vector<OrbitalElements> &elements,
                             std::vector<CelestialBody> &bodies,
                             const double julianDay,
                             const size_t planetDivideIndex) {
  for (size_t i = 0; i < planetDivideIndex; i++) {
    populateStateVectors(elements[i], bodies[i], julianDay);
  }
};


void drawBodies(const std::vector<CelestialBody> &planets, Picture &pic,
                size_t systemSize, bool finalPos) {

  for (CelestialBody p : planets) {
    Coord pos = p.pos / M_PER_AU;
    int x = scaleValue(pos.x, systemSize, pic.width() / 2) + pic.width() / 2;
    int y = scaleValue(-pos.y, systemSize, pic.width() / 2) + pic.width() / 2;
    if (p.name == "sun")
      pic.set(x, y, 255, 255, 0);
    else {
      if (!finalPos) {
        pic.set(x, y, 0, 255, 0);
      } else {
        pic.set(x, y, 255, 0, 0);
      }
    }
  }
}
