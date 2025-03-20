#include "../include/date.h"

// returns number of days between date and the J2000 Epoch
double Date::calcDaysSinceEpoch() {

  // intentional integer division
  double totDays = 367 * year - 7 * (year + (month + 9) / 12) / 4 -
                   3 * ((year + (month - 9) / 7) / 100 + 1) / 4 +
                   275 * month / 9 + day - 730515;
  totDays = totDays + universalTime / 24.0;

  return totDays - 1;
}
