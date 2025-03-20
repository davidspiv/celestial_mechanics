#include "../include/celestialBody.h"
#include "../include/coord.h"
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
void populateStateVectors(CelestialBody &planet) {

  // Orbital elements normalized to J2000
  const double a = planet.semiMajorAxis;
  const double e = planet.eccentricity;
  const double o = planet.longitudeOfAscendingNode;
  const double p = planet.longitudeOfPerihelion;
  const double i = planet.orbitalInclination;

  // Mean anomaly
  const double M = planet.meanAnomaly;
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
  planet.pos = {r * xh, r * yh, r * zh};

  // Standard gravitational parameter (mu)
  const double mu = G * (M_SUN + planet.mass);

  // Vis-Viva equation
  const double orbitalSpeed = sqrt(mu * (2.0 / r - 1.0 / a));

  // Heliocentric orbital velocity vector in 3D space, assuming the satellite's
  // motion is counterclockwise
  planet.vel = {orbitalSpeed * -yh, orbitalSpeed * xh, orbitalSpeed * zh};
}


// returns value as string regardless of data type
std::string getValueFromJSONLine(std::string line) {
  const size_t startIndex = line.rfind(':', line.length() - 1) + 2;
  const size_t valueLength = line.length() - startIndex;
  std::string value = line.substr(startIndex, valueLength);

  // remove quotes from json string values;
  if (line.find(": \"") == std::string::npos) {
    return value;
  }

  // ignore trailing commas
  const size_t endIndex = value.rfind('"') - 1;
  return value.substr(1, endIndex);
}

// reads planets.json into a dynamically allocated array of planet structs
std::vector<CelestialBody> populatePlanets() {
  const std::string firstKey = "\"name\": \"";
  std::fstream fileStream;
  std::string line;

  std::vector<CelestialBody> planets;

  fileStream.open("planets.json");

  while (std::getline(fileStream, line)) {
    const int objectStart = line.find(firstKey);

    // build planet
    if (objectStart > 0) {
      CelestialBody planet;
      planet.name = getValueFromJSONLine(line);

      std::getline(fileStream, line);
      planet.semiMajorAxis = std::stod(getValueFromJSONLine(line)) * M_PER_AU;

      std::getline(fileStream, line);
      planet.eccentricity = std::stod(getValueFromJSONLine(line));

      std::getline(fileStream, line);
      planet.orbitalInclination =
          toRadians(std::stod(getValueFromJSONLine(line)));

      std::getline(fileStream, line);
      planet.longitudeOfAscendingNode =
          toRadians(std::stod(getValueFromJSONLine(line)));

      std::getline(fileStream, line);
      planet.longitudeOfPerihelion =
          toRadians(std::stod(getValueFromJSONLine(line)));

      std::getline(fileStream, line);
      planet.meanAnomaly = toRadians(std::stod(getValueFromJSONLine(line)));

      std::getline(fileStream, line);
      planet.mass = std::stod(getValueFromJSONLine(line));

      populateStateVectors(planet);

      planets.emplace_back(planet);
    }
  }
  return planets;
}
