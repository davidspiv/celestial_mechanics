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

Coord calcAcc(const CelestialBody &p1, const CelestialBody &p2) {
  double gravitationalFactor = -G * (p1.mass + p2.mass);
  const double rSquared = p2.pos.magSquared(p1.pos); //[meters] distance

  return p1.pos * gravitationalFactor / (sqrt(rSquared) * rSquared);
}


Coord sumAcc(const CelestialBody &p) {
  const CelestialBody sun = {"Sun", {0, 0, 0}, {0, 0, 0}, M_SUN};
  Coord acc{0, 0, 0};

  acc += std::accumulate(
      planets.begin(), planets.end(), acc,
      [&](const Coord &totalAcc, const CelestialBody &other) {
        return p.mass != other.mass ? totalAcc + calcAcc(p, other) : totalAcc;
      });
  acc += calcAcc(p, sun);

  return acc;
};


CelestialBody rungeKuttaStep(const CelestialBody &p, int dt) {

  // Calculate Runge-Kutta terms
  Coord k1v = sumAcc(p) * dt;
  Coord k1r = p.vel * dt;
  CelestialBody k1CelestialBody{"", p.pos + k1r * 0.5, p.vel + k1v * 0.5,
                                p.mass};

  Coord k2v = sumAcc(k1CelestialBody) * dt;
  Coord k2r = (p.vel + k1v * 0.5) * dt;
  CelestialBody k2CelestialBody{"", p.pos + k2r * 0.5, p.vel + k2v * 0.5,
                                p.mass};

  Coord k3v = sumAcc(k2CelestialBody) * dt;
  Coord k3r = (p.vel + k2v * 0.5) * dt;
  CelestialBody k3CelestialBody{"", p.pos + k3r, p.vel + k3v, p.mass};

  Coord k4v = sumAcc(k3CelestialBody) * dt;
  Coord k4r = (p.vel + k3v) * dt;

  CelestialBody updatedBody = p;
  updatedBody.vel += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (1.0 / 6.0);
  updatedBody.pos += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * (1.0 / 6.0);

  return updatedBody;
}

int main() {
  Timer timer;
  const int picWidth = 500;
  const int picHeight = 500;
  const int picCenter = picWidth / 2;
  Picture pic(picWidth, picHeight, 0, 0, 0);
  pic.set(picCenter, picCenter, 255, 255, 0);

  double secondsPerYear = 31536000;
  const int dt = 600; // 10-minute intervals
  const int steps = secondsPerYear / dt;

  for (int i = 0; i < steps; i++) {
    std::vector<CelestialBody> updatedBodies;
    for (const auto &p : planets) {
      updatedBodies.emplace_back(rungeKuttaStep(p, dt));
    }
    planets = updatedBodies;

    for (CelestialBody p : planets) {
      Coord pos = p.pos / M_PER_AU;
      int x = scaleValue(pos.x, 31, picCenter) + picWidth / 2;
      int y = scaleValue(-pos.y, 31, picCenter) + picHeight / 2;
      pic.set(x, y, 0, 255, 0);
    }
  }

  for (CelestialBody p : planets) {
    Coord pos = p.pos / 1.496e+11;
    int x = scaleValue(pos.x, 31, picCenter) + picWidth / 2;
    int y = scaleValue(-pos.y, 31, picCenter) + picHeight / 2;
    pic.set(x, y, 255, 0, 0);
  }

  //   for (CelestialBody p : planets) {
  //     double distanceInM = sqrt(p.pos.magSquared(planets.at(2).pos));
  //     std::cout << p.name << ": " << distanceInM / M_PER_AU << std::endl;
  //   }


  std::cout << "----------------------------------------------------------\n";
  std::cout << setw(14) << "Name: " << planets.at(2).name << "\n";
  Coord acc = sumAcc(planets.at(2));
  std::cout << setw(14) << "Acc [m/s/s]: ";
  acc.print();
  Coord pos = planets.at(2).pos / M_PER_AU;
  std::cout << setw(14) << "Pos [AU]: ";
  pos.print();
  Coord vel = planets.at(2).vel / (M_PER_AU / S_PER_DAY);
  std::cout << setw(14) << "Vel [m/s]: ";
  vel.print();


  pic.save("result.png");
  return 0;
}
