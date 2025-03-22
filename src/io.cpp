#include "../include/celestialBody.h"
#include "../include/date.h"
#include "../include/getInitialPlanetState.h"
#include "../include/util.h"

#include <cmath>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>


// requests and returns text input from user
std::string getString(const std::string &prompt) {
  std::string input = "";
  std::cout << prompt;
  getline(std::cin, input);

  while (input == "") {
    std::cout << "No characters entered, try again." << std::endl;
    std::cout << prompt;
    getline(std::cin, input);
  }

  return input;
}


// requests date input from user and returns days since J200 epoch
double getDate() {

  bool isFormatted = false;
  Date date;

  do {
    int deliminatorCount = 0;
    std::string numAsString = "0";

    const std::string dateAsString = getString("Enter a date (MM/DD/YYYY): ");

    for (const char character : dateAsString) {
      if (character == '/') {
        deliminatorCount += 1;

        if (deliminatorCount == 1) {
          date.month = stoi(numAsString);
          numAsString = "0";
        } else if (deliminatorCount == 2) {
          date.day = stoi(numAsString);
          numAsString = "0";
        }
      } else {
        numAsString += character;
      }
    }

    date.year = stoi(numAsString);

    if (deliminatorCount == 2 && date.month > 0 && date.month <= 12 &&
        date.day > 0 && date.day <= 31) {
      isFormatted = true;
    } else {
      std::cout << "Date formatted incorrectly, try again";
    }
  } while (!isFormatted);

  return date.calcDaysSinceEpoch();
}


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


// displays formatted results
void printResults(const std::vector<CelestialBody> &planets) {
  const Coord earthPos = planets.at(2).pos;
  const Coord sunPos = planets.at(planets.size() - 1).pos;

  for (CelestialBody p : planets) {
    // if (p.name == "sun")
    //   continue;
    std::cout << "----------------------------------\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << std::setw(27) << "Name: " << p.name << "\n";
    std::cout << std::setw(27) << "Distance from Sun [AU]: ";
    std::cout << sqrt(p.pos.magSquared(sunPos)) / M_PER_AU << std::endl;
    std::cout << std::setw(27) << "Distance from Earth [AU]: ";
    std::cout << sqrt(p.pos.magSquared(earthPos)) / M_PER_AU << std::endl;
    std::cout << std::setw(27) << "Vel [km/sec]: ";
    std::cout << sqrt(p.vel.magSquared(Coord())) / M_PER_KM << std::endl;
  }
}


// gets answers from solutions.json and display formatted comparison
void printTest(const std::vector<CelestialBody> &bodies,
               const double julianDay) {
  std::vector<CelestialBody> solutionBodies;
  populateSolutions(solutionBodies);

  std::cout << "ERROR %\n\n";
  CelestialBody sun = bodies.at(bodies.size() - 1);
  for (size_t i = 0; i < bodies.size() - 1; i++) {
    std::cout << std::setw(7) << "NAME: " << bodies[i].name << '\n';
    double posObserved = bodies[i].pos.magSquared(sun.pos);
    double posExpected = solutionBodies[i].pos.magSquared(sun.pos);
    double velObserved = bodies[i].vel.magSquared(Coord());
    double velExpected = solutionBodies[i].vel.magSquared(Coord());

    double posError = (posObserved - posExpected) / posExpected * 100.0;
    double velError = (velObserved - velExpected) / velExpected * 100.0;

    std::cout << std::setw(7) << "POS: " << posError << '\n';
    std::cout << std::setw(7) << "VEL: " << velError << "\n\n";
  }
}
