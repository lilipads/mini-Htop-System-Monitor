#include <string>

#include "format.h"

using std::string;

inline string ToStringWithPadding(int time) {
  return (time < 10) ? '0' + std::to_string(time) : std::to_string(time);
}

string Format::ElapsedTime(long seconds) {
  /* returns a string in the format of HH:MM:SS */
  const int kSecondsInAnHour = 3600;
  const int kSecondsInAMinute = 60;

  int hour = seconds / kSecondsInAnHour;
  seconds -= hour * kSecondsInAnHour;
  int minute = seconds / kSecondsInAMinute;
  seconds -= minute * kSecondsInAMinute;

  return ToStringWithPadding(hour) + ':' + ToStringWithPadding(minute) + ":" +
         ToStringWithPadding(seconds);
}