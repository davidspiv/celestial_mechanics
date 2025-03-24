#include "../include/planet.h"
#include "../include/util.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


// returns value as string regardless of data type
std::string getValueFromJSONLine(const std::string &line) {
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


// reads planets.json into a parallel vectors
void populatePlanets(std::vector<OrbitalElements> &elements,
                     std::vector<CelestialBody> &bodies) {
  const std::string bodyStartKey = "\"name\": \"";
  std::fstream fileStream;
  std::string line;
  int numBodies = 0; // include sun

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
  int numBodies = 0; // include sun

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

  bodies.resize(numBodies);
}
