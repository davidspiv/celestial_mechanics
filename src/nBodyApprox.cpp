#include <algorithm>
#include <cmath>
#include <numeric>
#include <vector>

#include "../include/coord.h"
#include "../include/planet.h"
#include "../include/util.h"


// Updates heliocentric acceleration vectors for both bodies involved [m/s/s]
void calcAcc(const OrbitalStateVectors &p1, const OrbitalStateVectors &p2, Coord &acc1,
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
Coord sumAcc(const OrbitalStateVectors &p, size_t pIndex,
             const std::vector<OrbitalStateVectors> &planets) {

  static std::vector<Coord> accumulatedAcc(planets.size(), Coord());
  Coord netAcc = Coord();

  for (size_t i = pIndex + 1; i < planets.size(); i++) {
    calcAcc(p, planets[i], netAcc, accumulatedAcc[i]);
  }

  return netAcc;
}


// Approximate new position and velocity vectors for a given interval using
// 4th-Order Runge-Kutta. Returns updated body
OrbitalStateVectors rungeKuttaStep(size_t pIndex,
                             const std::vector<OrbitalStateVectors> &planets,
                             int dt) {

  const static double sixth = 1 / 6.0;
  OrbitalStateVectors p = planets[pIndex];

  const Coord k1v = sumAcc(p, pIndex, planets) * dt;
  const Coord k1r = p.vel * dt;
  const OrbitalStateVectors k1Body{"", p.pos + k1r * 0.5, p.vel + k1v * 0.5, p.mass};

  const Coord k2v = sumAcc(k1Body, pIndex, planets) * dt;
  const Coord k2r = (p.vel + k1v * 0.5) * dt;
  const OrbitalStateVectors K2Body{"", p.pos + k2r * 0.5, p.vel + k2v * 0.5, p.mass};

  const Coord k3v = sumAcc(K2Body, pIndex, planets) * dt;
  const Coord k3r = (p.vel + k2v * 0.5) * dt;
  const OrbitalStateVectors K3Body{"", p.pos + k3r, p.vel + k3v, p.mass};

  const Coord k4v = sumAcc(K3Body, pIndex, planets) * dt;
  const Coord k4r = (p.vel + k3v) * dt;

  p.vel += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * sixth;
  p.pos += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * sixth;

  return p;
}


void updateBodies(std::vector<OrbitalStateVectors> &planets, const int dt) {
  std::vector<OrbitalStateVectors> updatedBodies(planets.size());

  for (size_t i = 0; i < planets.size(); i++) {
    updatedBodies[i] = rungeKuttaStep(i, planets, dt);
  }

  planets = updatedBodies;
};
