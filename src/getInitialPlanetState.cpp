#include "../include/celestialBody.h"
#include "../include/coord.h"
#include "../include/io.h"
#include "../include/util.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


// returns numerical approximation of Eccentric Anomaly (E) using the
// Newton-Raphson method
double calcEccentricAnomaly(double eccentricity, double meanAnomaly) {
  const double e = eccentricity;
  const double M = meanAnomaly;
  auto isConverging = [](int count) { return count < 19; };

  // Kepler's equation
  double E = M + e * sin(M) * (1 + e * cos(M));

  // Newton's method
  double delta;
  int iterationCount = 0;
  do {
    const double E1 = E - (E - e * sin(E) - M) / (1 - e * cos(E));
    delta = abs(E1 - E);
    E = E1;
    iterationCount++;
  } while (delta >= 0.00001 && isConverging(iterationCount));

  // Newton's method does not work on hyperbolic orbits
  if (!isConverging(iterationCount)) {
    throw std::domain_error(
        "eccentricity too high. unable to converge\ndelta: " +
        std::to_string(delta));
  }

  return E;
}


// calculates heliocentric position and velocity vectors
void populateStateVectors(const OrbitalElements &element, CelestialBody &body) {
  // Orbital elements normalized to J2000
  const double a = element.semiMajorAxis;
  const double e = element.eccentricity;
  const double o = element.longitudeOfAscendingNode;
  const double p = element.longitudeOfPerihelion;
  const double i = element.orbitalInclination;

  // Mean anomaly
  const double M = element.meanAnomaly;
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


// reads planets.json into a parallel vectors
void populatePlanets(std::vector<OrbitalElements> &elements,
                     std::vector<CelestialBody> &bodies) {
  const std::string firstKey = "\"name\": \"";
  std::fstream fileStream;
  std::string line;
  int numPlanets = 0;

  fileStream.open("planets.json");

  while (std::getline(fileStream, line)) {
    const int objectStart = line.find(firstKey);

    // build element
    if (objectStart > 0) {
      OrbitalElements element;
      CelestialBody body;

      body.name = getValueFromJSONLine(line);

      std::getline(fileStream, line);
      element.semiMajorAxis = std::stod(getValueFromJSONLine(line)) * M_PER_AU;

      std::getline(fileStream, line);
      element.eccentricity = std::stod(getValueFromJSONLine(line));

      std::getline(fileStream, line);
      element.orbitalInclination =
          toRadians(std::stod(getValueFromJSONLine(line)));

      std::getline(fileStream, line);
      element.longitudeOfAscendingNode =
          toRadians(std::stod(getValueFromJSONLine(line)));

      std::getline(fileStream, line);
      element.longitudeOfPerihelion =
          toRadians(std::stod(getValueFromJSONLine(line)));

      std::getline(fileStream, line);
      element.meanAnomaly = toRadians(std::stod(getValueFromJSONLine(line)));

      std::getline(fileStream, line);
      body.mass = std::stod(getValueFromJSONLine(line));

      populateStateVectors(element, body);

      elements.emplace_back(element);
      bodies.emplace_back(body);

      numPlanets++;
    }
  }

  const CelestialBody sun = {"sun", Coord(), Coord(), M_SUN};
  bodies.emplace_back(sun);

  elements.resize(numPlanets);
  bodies.resize(numPlanets);
}


// approximates system size, assumes eccentricity is low
size_t approxSystemSize(const std::vector<OrbitalElements> &elements) {
  double systemSize = 0.0;
  for (auto p : elements) {
    if (p.semiMajorAxis > systemSize) {
      systemSize = p.semiMajorAxis;
    }
  }
  return size_t(std::ceil(systemSize / M_PER_AU));
}
