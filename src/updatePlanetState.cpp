#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

#include "../include/celestialBody.h"
#include "../include/coord.h"
#include "../include/picture.h"
#include "../include/util.h"


// Updates heliocentric acceleration vectors for both bodies involved [m/s/s]
void calcAcc(const CelestialBody &p1, const CelestialBody &p2, Coord &acc1,
             Coord &acc2) {
  const Coord r = p2.pos - p1.pos;
  const double distanceSquared = r.x * r.x + r.y * r.y + r.z * r.z;
  const double invDistanceCubed =
      G / (distanceSquared * std::sqrt(distanceSquared));

  acc1 += r * (invDistanceCubed * p2.mass);
  acc2 -= r * (invDistanceCubed * p1.mass);
}


// Adds together acceleration vectors produced by the gravitational force of
// the sun, and for every planet except itself
Coord sumAcc(const CelestialBody &p, size_t pIndex,
             const std::vector<CelestialBody> &planets) {

  static std::vector<Coord> accBuffer(planets.size(), Coord());
  Coord acc = Coord();

  for (size_t i = pIndex; i < planets.size(); i++) {
    if (i != pIndex) {
      calcAcc(p, planets[i], acc, accBuffer[i]);
    }
  }

  return acc;
}

// Approximate new position and velocity vectors for a given interval using
// 4th-Order Runge-Kutta. Returns updated body
CelestialBody rungeKuttaStep(size_t pIndex,
                             const std::vector<CelestialBody> &planets,
                             int dt) {

  CelestialBody p = planets[pIndex];

  const Coord k1v = sumAcc(p, pIndex, planets) * dt;
  const Coord k1r = p.vel * dt;
  const CelestialBody k1Body{"", p.pos + k1r * 0.5, p.vel + k1v * 0.5, p.mass};

  const Coord k2v = sumAcc(k1Body, pIndex, planets) * dt;
  const Coord k2r = (p.vel + k1v * 0.5) * dt;
  const CelestialBody K2Body{"", p.pos + k2r * 0.5, p.vel + k2v * 0.5, p.mass};

  const Coord k3v = sumAcc(K2Body, pIndex, planets) * dt;
  const Coord k3r = (p.vel + k2v * 0.5) * dt;
  const CelestialBody K3Body{"", p.pos + k3r, p.vel + k3v, p.mass};

  const Coord k4v = sumAcc(K3Body, pIndex, planets) * dt;
  const Coord k4r = (p.vel + k3v) * dt;

  p.vel += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (1 / 6.0);
  p.pos += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * (1 / 6.0);

  return p;
}


void drawBodies(const std::vector<CelestialBody> &planets, Picture &pic,
                int picCenter, bool finalPos) {

  // approximates system size, assumes eccentricity is low!
  double systemSize = 0.0;
  for (auto p : planets) {
    if (p.semiMajorAxis > systemSize) {
      systemSize = p.semiMajorAxis;
    }
  }
  systemSize = size_t(std::ceil(systemSize / M_PER_AU));

  for (CelestialBody p : planets) {
    Coord pos = p.pos / M_PER_AU;
    int x = scaleValue(pos.x, systemSize, picCenter) + picCenter;
    int y = scaleValue(-pos.y, systemSize, picCenter) + picCenter;
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


std::vector<CelestialBody>
updateBodies(const std::vector<CelestialBody> &planets, const int dt) {
  std::vector<CelestialBody> updatedBodies;

  for (size_t i = 0; i < planets.size(); i++) {
    updatedBodies.push_back(rungeKuttaStep(i, planets, dt));
  }

  return updatedBodies;
};
