#ifndef PROCESSOR_H
#define PROCESSOR_H

class Processor {
 public:
  float Utilization();

 private:
   long previous_utilized_time = 0;
   long previous_total_time = 0;
};

#endif