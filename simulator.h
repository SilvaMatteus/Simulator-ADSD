
#include <vector>
#include <string>
#include <random>
#include <ctime>

// distribution types
#define NORMAL "normal"
#define UNIFORME "uniforme"
#define EXPONENTIAL "exponential"

// distribution parameters
#define MEAN   1
#define STDDEV 0.1
#define ROLLS  1000000

class Simulator {
  int next = -1;
  int end = -1;
  std::vector<int> dist_end;
  int moment = 0;
  bool status_server = true;

  std::string distribution_type = NORMAL;
  std::vector<int> queue;
  int average_time_value = 4;
  int duration = 5;
  int repetitions = 1;
  int attend_time_average = 0;
  int waiting_average = 0;
  int requests_received = 0;
  int requests_attended = 0;

private:

    time_t current_time() {
      return time(NULL);
    }

public:

    void run() {
      // TODO Set paramenters
    }

    void simular() {
      time_t start;
      for (int i = 0; i < repetitions; i++) {
          time(&start); // get current time, same as timer = time(NULL)
          int queue_requests = 0;
          int queue_counter = 0;
          std::string result;
          std::cout << "repetition #" << i << ": \n";

          scheduling();
          while (duration > (current_time() - start)) {

            // TODO when it reaches the final moment (time) of the server's use

            // TODO when it reaches the arrival moment in the queue

          }
      }
    }

    void scheduling() {
      // TODO Calculate the next arrival time based on the distribution
    }

    void allocate_server() {
      // TODO Change server status and count completed requests
    }

    void status() {
      // TODO Print the simulator status
    }

};
