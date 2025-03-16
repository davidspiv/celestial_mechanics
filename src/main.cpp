#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

#include "../include/coord.h"
#include "../include/picture.h"


const double G = 6.67430e-11;   // Gravitational constant [m^3 kg^-1 s^-2]
const double M_sun = 1.9891e30; // Mass of the sun [kg]

size_t scaleValue(double x, size_t currMax, size_t newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}

struct PlanetState {
  Coord position;
  Coord velocity;
  double mass;
};

// Coord calcAcc(const PlanetState &p1, const PlanetState &p2) {
//   Coord r = {p2.position.x - p1.position.x, p2.position.y - p1.position.y,
//              p2.position.z - p1.position.z};
//   double distSq = r.x * r.x + r.y * r.y + r.z * r.z;
//   double dist = std::sqrt(distSq);
//   if (dist == 0)
//     return {0, 0, 0};
//   double force = G * p2.mass / (distSq * dist);
//   return r * force;
// }

Coord calcAcc(const PlanetState &p1, const PlanetState &p2) {
  Coord rCoord = p1.position.rCoord(p2.position);
  double distSq = p1.position.distSquared(p2.position);

  if (distSq == 0)
    return {0, 0, 0};
  double force = p2.mass * G / (distSq * std::sqrt(distSq));
  return rCoord * force;
}

PlanetState rungeKuttaStep(const PlanetState &planet,
                           const std::vector<PlanetState> &planets, double dt) {

  auto calcNetAcc = [&](const PlanetState &p) {
    return std::accumulate(planets.begin(), planets.end(), Coord(0, 0, 0),
                           [&p](const Coord &acc, const PlanetState &other) {
                             return &p != &other ? acc + calcAcc(p, other)
                                                 : acc;
                           });
  };

  //   Coord netAcc = std::accumulate(
  //       planets.begin(), planets.end(), Coord(0, 0, 0),
  //       [&planet](const Coord &acc, const PlanetState &other) {
  //         return &planet != &other ? acc + calcAcc(planet, other) : acc;
  //       });
  // Calculate Runge-Kutta terms
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
      calcNetAcc({planet.position + k3r, planet.velocity + k3v, planet.mass}) *
      dt;
  Coord k4r = (planet.velocity + k3v) * dt;

  //   std::cout << "k1r: ";
  //   k1r.print();
  //   std::cout << "k2r: ";
  //   k2r.print();
  //   std::cout << "k3r: ";
  //   k3r.print();
  //   std::cout << "k4r: ";
  //   k4r.print();

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

  double dt = 3600;     // 1 hour in seconds
  int steps = 365 * 24; // Simulate 1 year with 1-hour steps

  Picture pic(300, 300, 0, 0, 0);

  for (int i = 0; i < 300; i++) {
    std::vector<PlanetState> newStates(planets.size());

    // Calculate new states simultaneously
    for (size_t j = 0; j < planets.size(); j++) {
      newStates[j] = rungeKuttaStep(planets[j], planets, dt);
    }

    planets = newStates;

    // Draw Earth's position
    Coord pos = planets.at(1).position / 1.496e+11;
    int width = 300;
    int height = 300;
    int x = scaleValue(pos.x, 2, 150) + width / 2.0;
    int y = scaleValue(pos.y, 2, 150) + height / 2.0;
    pic.set(y, i, 255, 255, 255);

    // Debug Prints
    // if (i % 24 == 0) { // Print once per day
    //   std::cout << "Day " << i / 24 << ":\n";
    //   Coord acc = calcAcc(planets.at(1), planets.at(0));
    //   std::cout << "Acceleration: ";
    //   acc.print();
    //   Coord vel = planets.at(1).velocity;
    //   std::cout << "Velocity: ";
    //   vel.print();
    //   Coord pos = planets.at(1).position / 1.496e+11;
    //   std::cout << "Position (AU): ";
    //   pos.print();
    //   std::cout << "-------------------------\n";
    // }
  }

  pic.save("result.png");
  return 0;
}
