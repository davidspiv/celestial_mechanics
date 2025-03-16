#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

#include "../include/coord.h"

const double G = 6.67430e-11;   // Gravitational constant [m^3 kg^-1 s^-2]
const double M_sun = 1.9891e30; // Mass of the sun [kg]

struct PlanetState {
  Coord position;
  Coord velocity;
  double mass;
};

Coord calcAcc(const PlanetState &p1, const PlanetState &p2) {
  Coord rCoord = p1.position.rCoord(p2.position);
  double distSq = p1.position.distSquared(p2.position);

  if (distSq == 0)
    return {0, 0, 0};
  double force = p2.mass * G / (distSq * std::sqrt(distSq));
  return rCoord * force;
}

PlanetState rungeKuttaStep(const PlanetState &planet,
                           const std::vector<PlanetState> &planets, double
                           dt) {
  auto calcNetAcc = [&](const PlanetState &p) {
    return std::accumulate(planets.begin(), planets.end(), Coord(0, 0, 0),
                           [&p](const Coord &acc, const PlanetState &other) {
                             return &p != &other ? acc + calcAcc(p, other)
                                                 : acc;
                           });
  };

  Coord k1v = calcNetAcc(planet) * dt;

  Coord k1r = planet.velocity * dt;

  Coord k2v = calcNetAcc({planet.position + k1r * 0.5,
                          planet.velocity + k1v * 0.5, planet.mass}) *
              dt;
  Coord k2r = (planet.velocity + k1v * 0.5) * dt;

  Coord k3v = calcNetAcc({planet.position + k2r * 0.5,
                          planet.velocity + k2v * 0.5, planet.mass}) *
              dt;
  Coord k3r = (planet.velocity + k2v * 0.5) * dt;

  Coord k4v =
      calcNetAcc({planet.position + k3r, planet.velocity + k3v, planet.mass})
      * dt;

  Coord k4r = (planet.velocity + k3v) * dt;

  PlanetState newPlanet = planet;
  newPlanet.velocity += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (1.0 / 6.0);
  newPlanet.position += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * (1.0 / 6.0);

  return newPlanet;
}

int main() {
  // Example: Sun and Earth
  std::vector<PlanetState> planets = {
      {{0, 0, 0}, {0, 0, 0}, M_sun},                // Sun
      {{1.4959e11, 0, 0}, {0, 29780, 0}, 5.9722e24} // Earth
  };

  double dt = 3600; // 1 hour in seconds
                    //   int steps = 365 * 24;  // 1 year with 1-hour steps

  for (int i = 0; i < 1; i++) {
    std::vector<PlanetState> newStates;
    for (const auto &planet : planets) {
      newStates.push_back(rungeKuttaStep(planet, planets, dt));
    }
    planets = newStates;
  }

  return 0;
}
