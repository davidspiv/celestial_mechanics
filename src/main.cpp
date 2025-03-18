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

const double M_PER_AU = 1.496e11;
const double M_PER_KM = 1000;

const double G = 6.67430e-11;    // Gravitational constant
static double M_SUN = 1.9891e30; // [kg]

vector<CelestialBody> planets = populatePlanets();

size_t scaleValue(double x, size_t currMax, size_t newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}

Coord calcAcc(const CelestialBody &p1, const CelestialBody &p2) {
  double gravitationalFactor = -G * (p1.mass + p2.mass);
  const double rSquared = p2.pos.distSquared(p1.pos); //[meters] distance

  return p1.pos * gravitationalFactor / (sqrt(rSquared) * rSquared);
}

Coord getTotalAcc(const CelestialBody &p) {
  static CelestialBody sun = {"Sun", {0, 0, 0}, {0, 0, 0}, M_SUN};
  Coord acc{0, 0, 0};
  acc = std::accumulate(planets.begin(), planets.end(), acc,
                        [&](const Coord &totalAcc, const CelestialBody &other) {
                          return p.mass != other.mass
                                     ? totalAcc + calcAcc(p, other)
                                     : totalAcc;
                        });

  acc += calcAcc(p, sun);
  return acc;
};

CelestialBody rungeKuttaStep(const CelestialBody &p, int dt) {

  // Calculate Runge-Kutta terms
  Coord k1v = getTotalAcc(p) * dt;
  Coord k1r = p.vel * dt;
  CelestialBody k1CelestialBody{"", p.pos + k1r * 0.5, p.vel + k1v * 0.5,
                                p.mass};

  Coord k2v = getTotalAcc(k1CelestialBody) * dt;
  Coord k2r = (p.vel + k1v * 0.5) * dt;
  CelestialBody k2CelestialBody{"", p.pos + k2r * 0.5, p.vel + k2v * 0.5,
                                p.mass};

  Coord k3v = getTotalAcc(k2CelestialBody) * dt;
  Coord k3r = (p.vel + k2v * 0.5) * dt;
  CelestialBody k3CelestialBody{"", p.pos + k3r, p.vel + k3v, p.mass};

  Coord k4v = getTotalAcc(k3CelestialBody) * dt;
  Coord k4r = (p.vel + k3v) * dt;

  CelestialBody updatedBody = p;
  updatedBody.vel += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (1.0 / 6.0);
  updatedBody.pos += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * (1.0 / 6.0);

  return updatedBody;
}

int main() {
  const int picWidth = 1000;
  const int picHeight = 1000;
  const int picCenter = picWidth / 2;
  Picture pic(picWidth, picHeight, 0, 0, 0);
  pic.set(picCenter, picCenter, 255, 0, 0);

  for (size_t i = 0; i < planets.size(); i++) {
    getInitialPlanetState(planets.at(i));
    getPeriod(planets.at(i));
    // planets.at(i).mass = 5.97237e24;
    // getMass(planets.at(i));
  }

  double secondsPerYear = 31536000;
  int dt = 600; // 10-minute intervals
  const int steps = secondsPerYear / dt;

  for (int i = 0; i < steps * 10; i++) {
    std::vector<CelestialBody> updatedBody;
    for (const auto &p : planets) {
      updatedBody.push_back(rungeKuttaStep(p, dt));
    }
    planets = updatedBody;

    for (CelestialBody p : planets) {
      Coord pos = p.pos / M_PER_AU;
      int x = scaleValue(pos.x, 30, picCenter) + picWidth / 2;
      int y = scaleValue(-pos.y, 30, picCenter) + picHeight / 2;
      pic.set(x, y, 0, 255, 0);
    }
  }

  //   for (CelestialBody p : planets) {
  //     Coord pos = p.pos / 1.496e+11;
  //     int x = scaleValue(pos.x, 30, 150) + picWidth / 2;
  //     int y = scaleValue(-pos.y, 30, 150) + picHeight / 2;
  //     pic.set(x, y, 0, 255, 0);
  //   }

  for (CelestialBody p : planets) {
    double distanceInM = sqrt(p.pos.distSquared(planets.at(2).pos));
    std::cout << p.name << ": " << distanceInM / M_PER_AU << std::endl;
  }


  //   for (CelestialBody body : planets) {
  //     std::cout <<
  //     "----------------------------------------------------------\n";
  //     std::cout << setw(14) << "Name: " << body.name << "\n";
  //     Coord acc = getTotalAcc(body);
  //     std::cout << setw(14) << "Acc [m/s/s]: ";
  //     acc.print();
  //     Coord vel = body.vel / M_PER_KM;
  //     std::cout << setw(14) << "Vel [m/s]: ";
  //     vel.print();
  //     Coord pos = body.pos / M_PER_AU;
  //     std::cout << setw(14) << "Pos [AU]: ";
  //     pos.print();
  //   }

  pic.save("result.png");
  return 0;
}
