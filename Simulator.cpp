
#include <iostream>
#include <string>
#include "simulator.h"

using namespace std;

int main(int argc, char *argv[]) {

  if (argc < 3) {
    cout << "Usage: <distibution-type> <number-of-repetitions>\n";
    return 0;
  }

  string distribution_type = argv[1];
  int repetitions = atoi(argv[2]);

  Simulator s = Simulator(distribution_type, repetitions);
  s.run();

  return 0;
}
