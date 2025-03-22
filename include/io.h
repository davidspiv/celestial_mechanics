#ifndef IO_H
#define IO_H

#include <string>
#include <vector>

#include "celestialBody.h"

// requests and returns text input from user
std::string getString(const std::string &prompt);

// requests date input from user and returns days since J200 epoch
double getDate();

// returns value as string regardless of data type
std::string getValueFromJSONLine(const std::string &line);

// displays formatted results
void printResults(const std::vector<CelestialBody> &planets);


void printTest(const std::vector<CelestialBody> &bodies,
               const std::vector<CelestialBody> &solutionBodies);

#endif
