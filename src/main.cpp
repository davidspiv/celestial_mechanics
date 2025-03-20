#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "../include/celestialBody.h"
#include "../include/coord.h"
#include "../include/getInitialPlanetState.h"
#include "../include/picture.h"
#include "../include/timer.h"
#include "../include/util.h"

vector<CelestialBody> planets = populatePlanets();
const CelestialBody sun = {"Sun", {0, 0, 0}, {0, 0, 0}, M_SUN};

// Returns heliocentric acceleration vector [m/s/s]
Coord calcAcc(const CelestialBody &p1, const CelestialBody &p2) {
  double gravitationalFactor = G * (p1.mass + p2.mass);
  const double distSquared = p2.pos.magSquared(p1.pos);

  // Newton's law of universal gravitation
  return p1.pos * -gravitationalFactor / (sqrt(distSquared) * distSquared);
}


// Adds together acceleration vectors produced by the gravitational force of the
// sun, and for every planet except itself
Coord sumAcc(const CelestialBody &p) {
  const Coord acc = calcAcc(p, sun);

  return std::accumulate(
      planets.begin(), planets.end(), acc,
      [&](const Coord &totalAcc, const CelestialBody &other) {
        return p.mass != other.mass ? totalAcc + calcAcc(p, other) : totalAcc;
      });
};

// Approximate new state vectors for a given interval using 4th-Order
// Runge-Kutta. Returns updated Celestial Body
CelestialBody rungeKuttaStep(const CelestialBody &p, int dt) {
  const Coord k1v = sumAcc(p) * dt;
  const Coord k1r = p.vel * dt;
  const CelestialBody k1CelestialBody{"", p.pos + k1r * 0.5, p.vel + k1v * 0.5,
                                      p.mass};

  const Coord k2v = sumAcc(k1CelestialBody) * dt;
  const Coord k2r = (p.vel + k1v * 0.5) * dt;
  const CelestialBody k2CelestialBody{"", p.pos + k2r * 0.5, p.vel + k2v * 0.5,
                                      p.mass};

  const Coord k3v = sumAcc(k2CelestialBody) * dt;
  const Coord k3r = (p.vel + k2v * 0.5) * dt;
  const CelestialBody k3CelestialBody{"", p.pos + k3r, p.vel + k3v, p.mass};

  const Coord k4v = sumAcc(k3CelestialBody) * dt;
  const Coord k4r = (p.vel + k3v) * dt;

  CelestialBody updatedBody = p;
  updatedBody.vel += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (1 / 6.0);
  updatedBody.pos += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * (1 / 6.0);

  return updatedBody;
}

int main() {
  Timer timer;
  const int picSideLength = 500;
  const int picCenter = picSideLength / 2;
  Picture pic(picSideLength, picSideLength, 0, 0, 0);
  pic.set(picCenter, picCenter, 255, 255, 0);

  const int dt = 600; // 10-minute intervals
  const int steps = SEC_PER_YR / dt;

  for (int i = 0; i < steps; i++) {
    std::vector<CelestialBody> updatedBodies;
    std::transform(
        planets.begin(), planets.end(), std::back_inserter(updatedBodies),
        [dt](const CelestialBody &p) { return rungeKuttaStep(p, dt); });
    planets = updatedBodies;

    for (CelestialBody p : planets) {
      Coord pos = p.pos / M_PER_AU;
      int x = scaleValue(pos.x, 31, picCenter) + picCenter;
      int y = scaleValue(-pos.y, 31, picCenter) + picCenter;
      pic.set(x, y, 0, 255, 0);
    }
  }

  for (CelestialBody p : planets) {
    Coord pos = p.pos / 1.496e+11;
    int x = scaleValue(pos.x, 31, picCenter) + picCenter;
    int y = scaleValue(-pos.y, 31, picCenter) + picCenter;
    pic.set(x, y, 255, 0, 0);
  }
  pic.save("result.png");

  //   for (CelestialBody p : planets) {
  //     double distanceInM = sqrt(p.pos.magSquared(planets.at(2).pos));
  //     std::cout << p.name << ": " << distanceInM / M_PER_AU << std::endl;
  //   }


  for (CelestialBody p : planets) {
    std::cout << "----------------------------------------------------------\n";
    std::cout << setw(14) << "Name: " << planets.at(0).name << "\n";
    Coord acc = sumAcc(planets.at(0));
    std::cout << setw(14) << "Acc [m/s/s]: ";
    acc.print();
    Coord pos = planets.at(0).pos / M_PER_AU;
    std::cout << setw(14) << "Pos [AU]: ";
    pos.print();
    Coord vel = planets.at(0).vel / (M_PER_AU / SEC_PER_DAY);
    std::cout << setw(14) << "Vel [AU/day]: ";
    vel.print();
  }
}
