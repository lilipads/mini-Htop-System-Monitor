#include <string>

#include "format.h"

using std::string;

inline string ToStringWithPadding(int time) {
  return (time < 10) ? '0' + std::to_string(time) : std::to_string(time);
}

string Format::ElapsedTime(long seconds) {
  /* returns a string in the format of HH:MM:SS */
  const int SecondsInAnHour = 3600;
  const int SecondsInAMinute = 60;

  int hour = seconds / SecondsInAnHour;
  seconds -= hour * SecondsInAnHour;
  int minute = seconds / SecondsInAMinute;
  seconds -= minute * SecondsInAMinute;

  return ToStringWithPadding(hour) + ':' + ToStringWithPadding(minute) + ":" +
         ToStringWithPadding(seconds);
}