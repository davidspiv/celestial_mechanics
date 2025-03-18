#include "../include/celestialBody.h"
#include "../include/coord.h"
#include "../include/util.h"

#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

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
      planet.semiMajorAxis = std::stod(getValueFromJSONLine(line));

      std::getline(fileStream, line);
      planet.eccentricity = std::stod(getValueFromJSONLine(line));

      std::getline(fileStream, line);
      planet.orbitalInclination = std::stod(getValueFromJSONLine(line));

      std::getline(fileStream, line);
      planet.longitudeOfAscendingNode = std::stod(getValueFromJSONLine(line));

      std::getline(fileStream, line);
      planet.longitudeOfPerihelion = std::stod(getValueFromJSONLine(line));

      std::getline(fileStream, line);
      planet.meanAnomaly = std::stod(getValueFromJSONLine(line));


      planet.period = 0;
      planet.pos = {0, 0, 0};
      planet.vel = {0, 0, 0};
      planet.mass = 0;

      planets.push_back(planet);
    }
  }
  return planets;
}


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

  const double G = 6.67430e-11;      // Gravitational constant
  static double M_SUN = 1.988416e30; // [kg]
  const double M_PER_AU = 149597870700;

  // orbital elements normalized to J2000
  const double a = planet.semiMajorAxis * M_PER_AU;
  const double e = planet.eccentricity;
  const double o = toRadians(planet.longitudeOfAscendingNode);
  const double p = toRadians(planet.longitudeOfPerihelion);
  const double i = toRadians(planet.orbitalInclination);

  // normalized to specified date
  const double M = toRadians(planet.meanAnomaly);
  const double E = calcEccentricAnomaly(e, M);

  // position in 2d orbital plane
  const double xv = a * (cos(E) - e);
  const double yv = a * (sqrt(1.0 - e * e) * sin(E));

  // The True Anomaly (v) is the final angle we need to define the position
  // of a satellite in orbit, the other two being Eccentric Anomaly (E) and
  // Mean Anomaly (M).
  const double v = atan2(yv, xv);

  // The radius vector (r) is the distance of the satellite to the focal point
  // of the ellipse, in this case the sun.
  const double r = sqrt(xv * xv + yv * yv);

  // heliocentric 3d cartesian coordinates
  const double xh =
      r * (cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i));
  const double yh =
      r * (sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i));
  const double zh = r * (sin(v + p - o) * sin(i));

  planet.pos = {xh, yh, zh};

  // Calculate the period and gravitational parameter
  const double mu = G * (M_SUN + planet.mass);

  // Calculate the orbital velocity magnitude (vis-viva equation)
  const double orbitalVel = sqrt(mu * (2.0 / r - 1.0 / a));

  // Velocity in 3D space (perpendicular to radius in the orbital plane)
  const double vxh = sin(o) * cos(v + p - o) + cos(o) * sin(v + p - o) * cos(i);
  const double vyh = cos(o) * cos(v + p - o) - sin(o) * sin(v + p - o) * cos(i);
  const double vzh = sin(v + p - o) * sin(i);


  planet.vel = {orbitalVel * -vxh, orbitalVel * vyh, orbitalVel * vzh};
}
