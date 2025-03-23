#include "../include/planet.h"
#include "../include/coord.h"
#include "../include/io.h"
#include "../include/util.h"

#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>


static CelestialBody sun = {"sun", Coord(), Coord(), M_SUN};


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


double calcPeriod(const OrbitalElements &element, const CelestialBody &body) {
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


// calculates heliocentric position and velocity vectors
void populateStateVectors(const OrbitalElements &element, CelestialBody &body,
                          float daysSinceEpoch) {

//   double period = calcPeriod(element, body);

  const double normalizedMeanAnomaly =
      getNormalizedMeanAnomaly(element.meanAnomaly, element.period, daysSinceEpoch);

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


// reads planets.json into a parallel vectors
void populatePlanets(std::vector<OrbitalElements> &elements,
                     std::vector<CelestialBody> &bodies) {
  const std::string bodyStartKey = "\"name\": \"";
  std::fstream fileStream;
  std::string line;
  int numBodies = 1; // include sun

  fileStream.open("planets.json");

  while (std::getline(fileStream, line)) {
    const size_t objectStart = line.find(bodyStartKey);

    if (objectStart == std::string::npos)
      continue;

    // build element
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

	std::getline(fileStream, line);
    element.period = std::stod(getValueFromJSONLine(line));

    elements.emplace_back(element);
    bodies.emplace_back(body);

    numBodies++;
  }

  bodies.push_back(sun);

  elements.resize(numBodies);
  bodies.resize(numBodies);
}


void populateSolutions(std::vector<CelestialBody> &bodies,
                       const double julianDay) {

  const double normalizedJD = julianDay + 2451544.5;
  const bool isHalfDay = normalizedJD - static_cast<int>(normalizedJD) == 0.5;
  std::ostringstream ss;

  ss << "JD" << std::fixed << std::setprecision((isHalfDay ? 1 : 0))
     << normalizedJD;

  const std::string dataStartKey = ss.str();
  const std::string bodyStartKey = "\"name\": \"";
  std::fstream fileStream;
  std::string line;
  int numBodies = 1; // include sun

  fileStream.open("solutions.json");

  // get to correct data
  while (std::getline(fileStream, line)) {
    const size_t dataStart = line.find(dataStartKey);
    if (dataStart != std::string::npos)
      break;
  }

  while (std::getline(fileStream, line)) {
    const size_t objectStart = line.find(bodyStartKey);

    if (objectStart == std::string::npos)
      continue;

    // build element
    CelestialBody body;

    body.name = getValueFromJSONLine(line);

    std::getline(fileStream, line);
    std::getline(fileStream, line);
    body.pos.x = std::stod(getValueFromJSONLine(line)) * M_PER_KM;
    std::getline(fileStream, line);
    body.pos.y = std::stod(getValueFromJSONLine(line)) * M_PER_KM;
    std::getline(fileStream, line);
    body.pos.z = std::stod(getValueFromJSONLine(line)) * M_PER_KM;

    std::getline(fileStream, line);
    std::getline(fileStream, line);
    std::getline(fileStream, line);
    body.vel.x = std::stod(getValueFromJSONLine(line)) * M_PER_KM;
    std::getline(fileStream, line);
    body.vel.y = std::stod(getValueFromJSONLine(line)) * M_PER_KM;
    std::getline(fileStream, line);
    body.vel.z = std::stod(getValueFromJSONLine(line)) * M_PER_KM;

    std::getline(fileStream, line);
    std::getline(fileStream, line);
    body.mass = std::stod(getValueFromJSONLine(line));

    bodies.emplace_back(body);

    numBodies++;
  }

  bodies.push_back(sun);

  bodies.resize(numBodies);
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
