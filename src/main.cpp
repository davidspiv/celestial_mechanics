#include <cmath>
#include <iostream>
#include <numeric>
#include <vector>

#include "../include/coord.h"
#include "../include/picture.h"

struct Planet {
  Coord pos;
  Coord vel;
  double mass;
};

const double G = 6.67430e-11;    // Gravitational constant [m^3 kg^-1 s^-2]
const double mSun = 1.9891e30;   // Mass of the sun [kg]
const double mEarth = 5.9722e24; //[kg] mass of p

size_t scaleValue(double x, size_t currMax, size_t newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}

Coord calcAcc(const Planet &p1, const Planet &p2) {
  static double combinedMassAccScalar = -G * (mSun + mEarth);
  const double rSquared = p2.pos.distSquared(p1.pos); //[meters] distance

  return p1.pos * combinedMassAccScalar / (sqrt(rSquared) * rSquared);
}

Planet rungeKuttaStep(const Planet &p, const std::vector<Planet> &planets,
                      int dt) {

  auto calcNetAcc = [&](const Planet &p) {
    return std::accumulate(planets.begin(), planets.end(), Coord(0, 0, 0),
                           [&p](const Coord &acc, const Planet &other) {
                             return &p != &other ? acc + calcAcc(p, other)
                                                 : acc;
                           });
  };

  Planet sun = {{0, 0, 0}, {0, 0, 0}, mSun};

  //   auto calcNetAcc = [&](const Planet &p) { return calcAcc(p, sun); };

  // Calculate Runge-Kutta terms
  Coord k1v = calcNetAcc(p) * dt;
  Coord k1r = p.vel * dt;
  Planet k1Planet{p.pos + k1r * 0.5, p.vel + k1v * 0.5, p.mass};

  Coord k2v = calcNetAcc(k1Planet) * dt;
  Coord k2r = (p.vel + k1v * 0.5) * dt;
  Planet k2Planet{p.pos + k2r * 0.5, p.vel + k2v * 0.5, p.mass};

  Coord k3v = calcNetAcc(k2Planet) * dt;
  Coord k3r = (p.vel + k2v * 0.5) * dt;
  Planet k3Planet{p.pos + k3r, p.vel + k3v, p.mass};

  Coord k4v = calcNetAcc(k3Planet) * dt;
  Coord k4r = (p.vel + k3v) * dt;

  Planet updatedPlanet = p;
  updatedPlanet.vel += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (1.0 / 6.0);
  updatedPlanet.pos += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * (1.0 / 6.0);

  return updatedPlanet;
}

int main() {
  Planet earth = {{1.4959e11, 0, 0}, {0, 29780, 0}, mEarth};
  Planet mars = {{2.27942e11, 0, 0}, {0, 24100, 0}, mEarth};
  std::vector<Planet> planets{earth, mars};

  const int picWidth = 300;
  const int picHeight = 300;
  const int picCenter = picWidth / 2;
  Picture pic(picWidth, picHeight, 0, 0, 0);
  pic.set(picCenter, picCenter, 255, 0, 0);

  double secondsPerYear = 31536000;
  int dt = 600; // 10-minute intervals
  const int steps = secondsPerYear / static_cast<int>(dt);

  for (int i = 0; i < steps; i++) {
    std::vector<Planet> newPlanets;
    for (const auto &p : planets) {
      newPlanets.push_back(rungeKuttaStep(p, planets, dt));
    }
    planets = newPlanets;
    // planets.at(0) = rungeKuttaStep(planets.at(0), planets, dt);

    for (Planet p : planets) {
      Coord pos = p.pos / 1.496e+11;
      int x = scaleValue(pos.x, 2, 150) + picWidth / 2;
      int y = scaleValue(-pos.y, 2, 150) + picHeight / 2;
      pic.set(y, x, 0, 255, 0);
    }
  }


  pic.save("result.png");
  return 0;
}
