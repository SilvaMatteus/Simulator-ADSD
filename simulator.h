
#include <queue>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>

// distribution types
#define NORMAL "normal"
#define UNIFORME "uniforme"
#define EXPONENTIAL "exponential"

// distribution parameters
#define MEAN   1
#define STDDEV 0.1
#define ROLLS  1000000

class Simulator {

  double next = -1;
  double end = -1;
  double moment = 0;
  std::queue<int> dist_end;
  bool status_server = true;

  std::string distribution_type;
  std::queue<int> qu;
  int repetitions = 1;
  double average_time_value = 4.0;
  double duration = 5;
  double attend_time = 0;
  double server_start_time = 0;
  double attend_average_time = 0;
  double waiting_average = 0;
  int requests_received = 0;
  int requests_attended = 0;


public:

    Simulator() { }

    Simulator(std::string distribution_type, int repetitions) {
      this->distribution_type = distribution_type;
      this->duration = repetitions;
    }

    void run() {
      // TODO Set paramenters
      printf("running ...\n");
    }

    void simular() {
      double start;
      for (int i = 0; i < repetitions; i++) {
          start = 0; // get current time, same as timer = time(NULL)
          int queue_requests = 0;
          int queue_counter = 0;
          std::string result;
          std::cout << "repetition #" << i << ": \n";

          scheduling();
          while (duration > std::fabs(current_time() - start)) {

            // TODO when it reaches the final moment (time) of the server's use
            if (dist_end.back() <= std::fabs(current_time() - start))
            {
              requests_attended += 1;
              double exit_time = current_time();

              if (qu.empty())
                status_server = true;
              else {
                allocate_server();
                qu.pop();
              }

              attend_time += std::fabs(exit_time - server_start_time);
              attend_time /= double(CLOCKS_PER_SEC) * 1000; // ms
            }

            // TODO when it reaches the arrival moment in the queue
            if (next <= std::fabs(current_time() - start))
            {
              requests_received += 1;
              scheduling();
              if (status_server)
                allocate_server();
              else {
                qu.push(current_time());
                queue_requests += 1;
                queue_counter += qu.size();
              }
            }
          }

          attend_average_time = attend_time / requests_attended;
          waiting_average = double(queue_counter) / requests_received;

          status();
      }
    }

    void scheduling() {
      // TODO Calculate the next arrival time based on the distribution
      printf("%.5f\n", moment);
      // TODO Calculate the next arrival
      moment += 1.0;
    }

    void allocate_server() {
      // TODO Change server status and count completed requests
      status_server = false;
      end += 1;
      server_start_time = current_time();
    }

    void status() {
      // TODO Print the simulator status
      printf("Simulation { \n\t distribution_type: %s,", getTypeDistribution().c_str());
      printf("\n\t params: [],");
      printf("\n\t duration: %.6f,", duration);
      printf("\n\t requests_received: %d,", requests_received);
      printf("\n\t requests_attended: %d,", requests_attended);
      printf("\n\t waiting_average: %.6f,", waiting_average);
      printf("\n\t attend_average_time: %.6f", attend_average_time);
      printf("\n}\n");
    }

    std::string getTypeDistribution() {
      return this->distribution_type;
    }

  private:

    int current_time() {
      return clock();
    }

    void setTypeDistribution(std::string distribution_type) {
      this->distribution_type = distribution_type;
    }
};
