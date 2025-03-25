#ifndef IO_H
#define IO_H

#include <string>
#include <vector>

#include "planet.h"

// requests and returns text input from user
std::string getString(const std::string &prompt);


// requests date input from user and returns days since J200 epoch
double getDate();


// displays formatted results
void printResults(const std::vector<StateVector> &planets);


void printTest(const std::vector<StateVector> &bodies,
               const double daysSinceEpoch);

#endif
