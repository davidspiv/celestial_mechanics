#ifndef DATE_H
#define DATE_H

struct Date {
  int month;
  int day;
  int year;
  double universalTime = 0.0;

  double calcDaysSinceEpoch();
};

double getDate();

#endif
