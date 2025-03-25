#include "../include/coord.h"
#include "../include/io.h"
#include "../include/planet.h"
#include "../include/util.h"

#include <cmath>
#include <vector>


double calcPeriod(const OrbitalElements &element, const StateVector &body) {
  const double proportionalityConstant =
      (4 * pow(M_PI, 2)) / (G * (body.mass + M_SUN));

  return sqrt(proportionalityConstant * pow(element.semiMajorAxis, 3)) /
         SEC_PER_DAY;
}


double getNormalizedMeanAnomaly(const double meanAnomaly, const double period,
                                double daysSinceEpoch) {
  const double meanMotion = (2 * M_PI) / period; // Radians per day
  return normalizeRadians(meanAnomaly + meanMotion * daysSinceEpoch);
}


// returns numerical approximation of Eccentric Anomaly (E) using the
// Newton-Raphson method.
double calcEccentricAnomaly(double eccentricity, double meanAnomaly) {
  const double e = eccentricity;
  const double M = meanAnomaly;
  const double tolerance = 1e-8;
  const int maxIterations = 20;

  // Initial guess
  double E = (e < 0.8) ? M : M + e * sin(M) * (1 + e * cos(M));

  // Newton-Raphson method
  for (int iterationCount = 0; iterationCount < maxIterations;
       ++iterationCount) {
    double deltaE = (E - e * sin(E) - M) / (1 - e * cos(E));
    E -= deltaE;

    if (std::abs(deltaE) < tolerance) {
      return E;
    }
  }

  throw std::domain_error(
      "Failed to converge: eccentricity probably too high\n");
}


// calculates heliocentric position and velocity vectors
void calcStateVectors(const OrbitalElements &element, StateVector &body,
                      float daysSinceEpoch) {

  double period = calcPeriod(element, body);

  const double normalizedMeanAnomaly =
      getNormalizedMeanAnomaly(element.meanAnomaly, period, daysSinceEpoch);

  // Orbital elements normalized to J2000
  const double a = element.semiMajorAxis;
  const double e = element.eccentricity;
  const double o = element.longitudeOfAscendingNode;
  const double p = element.longitudeOfPerihelion;
  const double i = element.orbitalInclination;

  const double M = normalizedMeanAnomaly;
  const double E = calcEccentricAnomaly(e, M);

  // Position in 2D orbital plane
  const double xv = a * (cos(E) - e);
  const double yv = a * (sqrt(1.0 - e * e) * sin(E));

  // The True Anomaly (v)
  const double v = atan2(yv, xv);

  // The radius vector (r)
  const double r = sqrt(xv * xv + yv * yv);

  // vector components used for both position and velocity
  const double xh = cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i);
  const double yh = sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i);
  const double zh = sin(v + p - o) * sin(i);

  // Heliocentric position in 3D space
  body.pos = {r * xh, r * yh, r * zh};

  // Standard gravitational parameter (mu)
  const double mu = G * (M_SUN + body.mass);

  // Vis-Viva equation
  const double orbitalSpeed = sqrt(mu * (2.0 / r - 1.0 / a));

  // Heliocentric orbital velocity vector in 3D space, assuming the satellite's
  // motion is counterclockwise
  body.vel = {orbitalSpeed * -yh, orbitalSpeed * xh, orbitalSpeed * zh};
}

// One-body approximation
void keplerianApprox(const std::vector<OrbitalElements> &elements,
                     std::vector<StateVector> &bodies,
                     const double daysSinceEpoch) {

  for (size_t i = 0; i < bodies.size(); i++) {
    calcStateVectors(elements[i], bodies[i], daysSinceEpoch);
  }
};
