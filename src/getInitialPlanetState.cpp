#include "../include/celestialBody.h"
#include "../include/coord.h"
#include "../include/util.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>


void getPeriod(CelestialBody &p) {
  const double a = p.semiMajorAxis;
  p.period = (2 * M_PI * sqrt(pow(a, 3) / (G * M_SUN)));
}

// Calculate mass from period (if needed)
// void getMass(CelestialBody &p) {
//   const double a = p.semiMajorAxis;
//   p.mass = ((4 * M_PI * M_PI * pow(a, 3)) / (G * p.period * p.period));
// }

// Numerical approximation of Eccentric Anomaly (E) using the Newton-Raphson
// method
double calcEccentricAnomaly(double eccentricity, double meanAnomaly) {
  const double e = eccentricity;
  const double M = meanAnomaly;
  auto isConverging = [](int count) { return count < 19; };

  // the inverse of the standard form of Kepler's equation
  double E = M + e * sin(M) * (1 + e * cos(M));

  double delta;
  int iterationCount = 0;

  do {
    const double E1 = E - (E - e * sin(E) - M) / (1 - e * cos(E));
    delta = abs(E1 - E);
    E = E1;
    iterationCount++;
  } while (delta >= 0.000001 && isConverging(iterationCount));

  // failsafe, should never happen with current planet selection
  if (!isConverging(iterationCount)) {
    std::cout << "calcEccentricAnomaly() failed. unable to converge." << '\n'
              << "delta: " + std::to_string(delta);
    exit(1);
  }

  return E;
}

// calculates the heliocentric coordinates of the planet at the specified time
void getInitialPlanetState(CelestialBody &planet) {

  // Orbital elements normalized to J2000
  const double a = planet.semiMajorAxis;
  const double e = planet.eccentricity;
  const double o = planet.longitudeOfAscendingNode;
  const double p = planet.longitudeOfPerihelion;
  const double i = planet.orbitalInclination;

  // Mean anomaly
  const double M = planet.meanAnomaly; //
  const double E = calcEccentricAnomaly(e, M);

  // Position in 2D orbital plane
  const double xv = a * (cos(E) - e);
  const double yv = a * (sqrt(1.0 - e * e) * sin(E));

  // The True Anomaly (v)
  const double v = atan2(yv, xv);

  // The radius vector (r)
  const double r = sqrt(xv * xv + yv * yv);

  // Heliocentric 3D cartesian coordinates
  const double rxh =
      r * (cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i));
  const double ryh =
      r * (sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i));
  const double rzh = r * (sin(v + p - o) * sin(i));

  planet.pos = {rxh, ryh, rzh};

  // Calculate the gravitational parameter
  const double mu = G * (M_SUN + planet.mass);

  // Calculate the orbital velocity magnitude (vis-viva equation)
  const double orbitalVel = sqrt(mu * (2.0 / r - 1.0 / a));

  // Velocity in 3D space (perpendicular to radius in the orbital plane)
  const double vxh = sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i);
  const double vyh = cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i);
  const double vzh = sin(v + p - o) * sin(i);

  planet.vel = {orbitalVel * -vxh, orbitalVel * vyh, orbitalVel * vzh};
}


// returns value sans quotes regardless if its a string or a number
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

      // have to initialize all variables before passing object?
      planet.pos = {0, 0, 0};
      planet.vel = {0, 0, 0};
      planet.period = 0;

      getInitialPlanetState(planet);
      getPeriod(planet);

      planets.push_back(planet);
    }
  }
  return planets;
}
