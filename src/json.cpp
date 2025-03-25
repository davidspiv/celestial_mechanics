#include "../include/planet.h"
#include "../include/util.h"

#include <exception>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
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
                     std::vector<StateVector> &bodies) {
  const std::string bodyStartKey = "\"name\": \"";
  std::fstream fileStream;
  std::string line;
  int numBodies = 0;

  fileStream.open("planets.json");

  while (std::getline(fileStream, line)) {
    const size_t objectStart = line.find(bodyStartKey);

    if (objectStart == std::string::npos)
      continue;

    // build element
    OrbitalElements element;
    StateVector body;

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

    elements.emplace_back(element);
    bodies.emplace_back(body);

    numBodies++;
  }

  elements.resize(numBodies);
  bodies.resize(numBodies);
}


void populateSolutions(std::vector<StateVector> &bodies,
                       const double daysSinceEpoch) {

  const double julianDay = daysSinceEpoch + 2451544.5;
  const bool isHalfDay = julianDay - static_cast<int>(julianDay) == 0.5;
  std::ostringstream keyBuffer;

  keyBuffer << "JD" << std::fixed << std::setprecision((isHalfDay ? 1 : 0))
            << julianDay;

  const std::string dataStartKey = keyBuffer.str();
  const std::string bodyStartKey = "\"name\": \"";
  std::fstream fileStream;
  std::string line;
  int numBodies = 0;

  fileStream.open("solutions.json");

  // get to correct data
  while (std::getline(fileStream, line)) {
    const size_t dataStart = line.find(dataStartKey);
    if (dataStart != std::string::npos)
      break;
  }

  if (line.empty())
    throw std::domain_error("No test corresponding to input date found.\n");

  while (std::getline(fileStream, line) && line.find("]")) {
    const size_t objectStart = line.find(bodyStartKey);

    if (objectStart == std::string::npos)
      continue;

    // build element
    StateVector body;

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

void populateStateVectors(std::vector<StateVector> &bodies) {

  const std::string dataStartKey = "JD2451544.5";
  const std::string bodyStartKey = "\"name\": \"";
  std::fstream fileStream;
  std::string line;
  int bodiesIndex = 0;

  fileStream.open("solutions.json");

  // get to correct data
  while (std::getline(fileStream, line)) {
    const size_t dataStart = line.find(dataStartKey);
    if (dataStart != std::string::npos)
      break;
  }

  if (line.empty())
    throw std::domain_error("J2000 Epoch data not found.\n");

  while (std::getline(fileStream, line) &&
         line.find("]") == std::string::npos) {
    const size_t objectStart = line.find(bodyStartKey);

    if (objectStart == std::string::npos)
      continue;

    bodies.at(bodiesIndex).name = getValueFromJSONLine(line);

    std::getline(fileStream, line);
    std::getline(fileStream, line);
    bodies.at(bodiesIndex).pos.x =
        std::stod(getValueFromJSONLine(line)) * M_PER_KM;
    std::getline(fileStream, line);
    bodies.at(bodiesIndex).pos.y =
        std::stod(getValueFromJSONLine(line)) * M_PER_KM;
    std::getline(fileStream, line);
    bodies.at(bodiesIndex).pos.z =
        std::stod(getValueFromJSONLine(line)) * M_PER_KM;

    std::getline(fileStream, line);
    std::getline(fileStream, line);
    std::getline(fileStream, line);
    bodies.at(bodiesIndex).vel.x =
        std::stod(getValueFromJSONLine(line)) * M_PER_KM;
    std::getline(fileStream, line);
    bodies.at(bodiesIndex).vel.y =
        std::stod(getValueFromJSONLine(line)) * M_PER_KM;
    std::getline(fileStream, line);
    bodies.at(bodiesIndex).vel.z =
        std::stod(getValueFromJSONLine(line)) * M_PER_KM;

    std::getline(fileStream, line);
    std::getline(fileStream, line);
    bodies.at(bodiesIndex).mass = std::stod(getValueFromJSONLine(line));

    bodiesIndex++;
  }

  if (bodiesIndex != 8)
    throw std::range_error("Mismatch between json data and number of planets");
}
