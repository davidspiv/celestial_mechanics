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
const double mEarth = 5.9722e24; //[kg] mass of earth
const double combinedMassAccScalar = -G * (mSun + mEarth);

size_t scaleValue(double x, size_t currMax, size_t newMax) {
  if (!currMax) {
    throw std::invalid_argument("currMax cannot be zero");
  }
  return newMax * (x / currMax);
}

Coord calcAcc(const Planet &p1, const Planet &p2) {
  double rSquared = p2.pos.distSquared(p1.pos); //[meters] distance

  return p1.pos * combinedMassAccScalar / (sqrt(rSquared) * rSquared);
}

Planet rungeKuttaStep(const Planet &earth, const Planet &sun, double dt) {

  // Calculate Runge-Kutta terms
  Coord k1v = calcAcc(earth, sun) * dt;
  Coord k1r = earth.vel * dt;
  Planet k1Planet{earth.pos + k1r * 0.5, earth.vel + k1v * 0.5, earth.mass};

  Coord k2v = calcAcc(k1Planet, sun) * dt;
  Coord k2r = (earth.vel + k1v * 0.5) * dt;
  Planet k2Planet{earth.pos + k2r * 0.5, earth.vel + k2v * 0.5, earth.mass};

  Coord k3v = calcAcc(k2Planet, sun) * dt;
  Coord k3r = (earth.vel + k2v * 0.5) * dt;
  Planet k3Planet{earth.pos + k3r, earth.vel + k3v, earth.mass};

  Coord k4v = calcAcc(k3Planet, sun) * dt;
  Coord k4r = (earth.vel + k3v) * dt;

  Planet updatedPlanet = earth;
  updatedPlanet.vel += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (1.0 / 6.0);
  updatedPlanet.pos += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * (1.0 / 6.0);

  return updatedPlanet;
}

int main() {
  Planet sun = {{0, 0, 0}, {0, 0, 0}, mSun};
  Planet earth = {{1.4959e11, 0, 0}, {0, 29780, 0}, mEarth};
  std::vector<Planet> planets{sun, earth};

  const int picWidth = 300;
  const int picHeight = 300;
  const int picCenter = picWidth / 2;
  Picture pic(picWidth, picHeight, 0, 0, 0);
  pic.set(picCenter, picCenter, 255, 0, 0);

  double secondsPerYear = 31536000;
  double dt = 600.0; // 10-minute intervals
  const int steps = secondsPerYear / static_cast<int>(dt); // Adjust for new dt
  for (int i = 0; i < steps; i++) {

    planets.at(1) = rungeKuttaStep(planets.at(1), planets.at(0), dt);

    Coord pos = planets.at(1).pos / 1.496e+11;
    int x = scaleValue(pos.x, 2, 150) + picWidth / 2;
    int y = scaleValue(-pos.y, 2, 150) + picHeight / 2;
    pic.set(y, x, 0, 255, 0);
  }


  pic.save("result.png");
  return 0;
}
