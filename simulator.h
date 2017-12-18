
#include <vector>
#include <string>

// distribution types
#define NORMAL "normal"
#define UNIFORME "uniforme"
#define EXPONENTIAL "exponential"

// distribution parameters
#define MEAN 1
#define STDDEV 0.1
#define ROLLS 1000000

class Simulator {
  int next = -1;
  int end = -1;
  std::vector<int> dist_end;
  int moment = 0;
  bool status_server = true;

  std::string distribution_type = NORMAL;
  int average_time_value = 4;
  int duration = 5;
  int number_of_repetitions = 1;

public:
    void run() {

    }
}
