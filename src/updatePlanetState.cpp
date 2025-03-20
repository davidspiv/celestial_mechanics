#include <cmath>
#include <numeric>
#include <vector>

#include "../include/celestialBody.h"
#include "../include/coord.h"
#include "../include/util.h"

// Returns heliocentric acceleration vector [m/s/s]
Coord calcAcc(const CelestialBody &p1, const CelestialBody &p2) {
  double gravitationalFactor = G * (p1.mass + p2.mass);
  const double distSquared = p2.pos.magSquared(p1.pos);

  // Newton's law of universal gravitation
  return p1.pos * -gravitationalFactor / (sqrt(distSquared) * distSquared);
}


// Adds together acceleration vectors produced by the gravitational force of the
// sun, and for every planet except itself
Coord sumAcc(const CelestialBody &p,
             const std::vector<CelestialBody> &planets) {
  const CelestialBody sun = {"Sun", Coord(), Coord(), M_SUN};
  const Coord acc = calcAcc(p, sun);

  return std::accumulate(
      planets.begin(), planets.end(), acc,
      [&](const Coord &totalAcc, const CelestialBody &other) {
        return p.mass != other.mass ? totalAcc + calcAcc(p, other) : totalAcc;
      });
};

// Approximate new state vectors for a given interval using 4th-Order
// Runge-Kutta. Returns updated Celestial Body
CelestialBody rungeKuttaStep(const CelestialBody &p,
                             const std::vector<CelestialBody> &planets,
                             int dt) {
  const Coord k1v = sumAcc(p, planets) * dt;
  const Coord k1r = p.vel * dt;
  const CelestialBody k1CelestialBody{"", p.pos + k1r * 0.5, p.vel + k1v * 0.5,
                                      p.mass};

  const Coord k2v = sumAcc(k1CelestialBody, planets) * dt;
  const Coord k2r = (p.vel + k1v * 0.5) * dt;
  const CelestialBody k2CelestialBody{"", p.pos + k2r * 0.5, p.vel + k2v * 0.5,
                                      p.mass};

  const Coord k3v = sumAcc(k2CelestialBody, planets) * dt;
  const Coord k3r = (p.vel + k2v * 0.5) * dt;
  const CelestialBody k3CelestialBody{"", p.pos + k3r, p.vel + k3v, p.mass};

  const Coord k4v = sumAcc(k3CelestialBody, planets) * dt;
  const Coord k4r = (p.vel + k3v) * dt;

  CelestialBody updatedBody = p;
  updatedBody.vel += (k1v + k2v * 2.0 + k3v * 2.0 + k4v) * (1 / 6.0);
  updatedBody.pos += (k1r + k2r * 2.0 + k3r * 2.0 + k4r) * (1 / 6.0);

  return updatedBody;
}
