
#include <queue>
#include <vector>
#include <string>
#include <random>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <chrono>
#include <fstream>

// distribution types
#define NORMAL "normal"
#define UNIFORME "uniforme"
#define EXPONENTIAL "exponential"

// distribution parameters
#define MEAN   1
#define STDDEV 0.4
#define ROLLS  1000000

#define REP 10
#define EPS 5

class Simulator {

  double next = -1;
  double completed = -1;
  int moment = 0;
  std::vector<double> dist_end;
  std::vector<double> samples;
  bool status_server = true;

  std::string distribution_type;
  double mean;
  double stddev;
  double rolls;

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

    Simulator() {
      this->setTypeDistribution(NORMAL);
      this->setMean(MEAN);
      this->setStdDeviation(STDDEV);
      this->setNrOfRepetitions(REP);
      this->setRolls(ROLLS);
      this->setDuration(EPS);
    }

    Simulator(std::string distribution_type=NORMAL, int repetitions=REP) {
      this->setTypeDistribution(distribution_type);
      this->setMean(MEAN);
      this->setStdDeviation(STDDEV);
      this->setNrOfRepetitions(REP);
      this->setRolls(ROLLS);
      this->setDuration(EPS);
    }

    Simulator(std::string distribution_type=NORMAL, double mean=MEAN, double stddev=STDDEV, int rolls=ROLLS, int repetitions=REP, int duration=EPS) {
      this->setTypeDistribution(distribution_type);
      this->setMean(mean);
      this->setStdDeviation(stddev);
      this->setNrOfRepetitions(repetitions);
      this->setRolls(rolls);
      this->setDuration(duration);
    }

    void run() {
      // TODO Set paramenters
      printf("running ...\n");
      genExponentialRandomSamples(this->dist_end, MEAN, ROLLS);
      simular();
    }

    void simular() {
      double start;
      for (int i = 0; i < repetitions; i++)
      {
          std::vector<double> distribution(getRandomSamples());
          start = current_time();
          int queue_requests = 0;
          int queue_counter = 0;
          std::string result;

          this->moment = 0;
          this->next = -1;
          this->completed = -1;

          while (!this->qu.empty()) qu.pop();

          this->requests_received = 0;
          this->requests_attended = 0;
          this->waiting_average = 0;
          this->attend_average_time = 0;
          this->attend_time = 0;
          this->server_start_time = 0;

          std::cout << "repetition #" << (i+1) << ": \n";

          scheduling(distribution);

          while (duration > diff(current_time(), start) ) {

            double finished;
            if (this->completed < 0)
              finished = dist_end.back();
            else
              finished = dist_end[this->completed];

            if (finished <= diff(current_time(), start))
            {
              requests_attended += 1;
              double exit_time = current_time();

              if (qu.empty())
                status_server = true;
              else {
                allocate_server();
                qu.pop();
              }

              attend_time += diff(exit_time, server_start_time);

            }

            if (next <= diff(current_time(), start))
            {
              requests_received += 1;
              scheduling(distribution);
              if (status_server)
                allocate_server();
              else {
                qu.push(current_time());
                queue_requests += 1;
                queue_counter += qu.size();
              }
            }
          }

          if (requests_attended > 0)
            attend_average_time = attend_time / requests_attended;

          waiting_average = double(queue_counter) / double(requests_received);
          status();
      }
    }

    void scheduling(std::vector<double>& distribution) {
      if (this->moment < distribution.size())
        this->next = std::fabs(distribution[this->moment] );
      this->moment += 1;
    }

    void allocate_server() {
      this->status_server = false;
      this->completed += 1;
      this->server_start_time = current_time();
    }

    double getMean() {
      return this->mean;
    }

    double getStdDeviation() {
      return this->stddev;
    }

    int getNrOfRepetitions() {
      return this->rolls;
    }

    double getDuration() {
      return this->duration;
    }

    int getRequestsAttended() {
      return this->requests_attended;
    }

    int getRequestsReceived() {
      return this->requests_received;
    }

    double getWaitingAverageTime() {
      return this->waiting_average;
    }

    double getAttendAverageTime() {
      return this->attend_average_time;
    }

    void status() {
      // TODO Print the simulator status
      printf("Simulation { \n\t distribution_type: %s,", getTypeDistribution().c_str());
      printf("\n\t params: {\n\t\t mean: %.5f\n\t\t std_deviation: %.5f\n\t\t rolls: %d\n\t },", getMean(), getStdDeviation(), getNrOfRepetitions());
      printf("\n\t duration (ms): %.6f,", getDuration());
      printf("\n\t requests_received: %d,", getRequestsReceived());
      printf("\n\t requests_attended: %d,", getRequestsAttended());
      printf("\n\t waiting_average_time (ms): %.6f,", getWaitingAverageTime());
      printf("\n\t attend_average_time (ms): %.6f", getAttendAverageTime());
      printf("\n}\n");
    }

    std::string getTypeDistribution() {
      return this->distribution_type;
    }

    std::vector<double> getRandomSamples() {
      std::vector<double> samples;
      if (this->getTypeDistribution().compare(NORMAL) == 0) {
        genNormalRandomSamples(samples, this->mean, this->stddev, this->rolls);
      }
      else if (this->getTypeDistribution().compare(EXPONENTIAL) == 0) {
        genExponentialRandomSamples(samples, this->stddev, this->rolls);
      }
      else if (this->getTypeDistribution().compare(UNIFORME) == 0) {
        genUniformRandomSamples(samples, this->mean, this->stddev, this->rolls);
      }
      return samples;
    }

  private:

    void genNormalRandomSamples(std::vector<double>& samples, double mean, double stddev, int rolls) {
      std::default_random_engine generator;
      std::normal_distribution<double> distribution(mean, stddev);
      for (int i = 0; i < rolls; i++) {
        samples.push_back( distribution(generator) );
      }
    }

    void genExponentialRandomSamples(std::vector<double>& samples, double mean, int rolls) {
      double lambda = 1.0 / mean;
      std::default_random_engine generator;
      std::exponential_distribution<double> distribution(lambda);
      for (int i = 0; i < rolls; i++) {
        samples.push_back( distribution(generator) );
      }
    }

    void genUniformRandomSamples(std::vector<double>& samples, double mean, double stddev, int rolls) {
      std::default_random_engine generator;
      std::uniform_real_distribution<double> distribution(mean, stddev);
      for (int i = 0; i < rolls; i++) {
        samples.push_back( distribution(generator) );
      }
    }

    void setTypeDistribution(std::string distribution_type) {
      this->distribution_type = distribution_type;
    }

    void setMean(double mean) {
      this->mean = mean;
    }

    void setStdDeviation(double stddev) {
      this->stddev = stddev;
    }

    void setNrOfRepetitions(int repetitions) {
      this->repetitions = repetitions;
    }

    void setDuration(int duration) {
      this->duration = duration;
    }

    void setRolls(int rolls) {
      this->rolls = rolls;
    }

    double diff(double end, double start) {
        return std::fabs(end - start) / double(CLOCKS_PER_SEC) * 1000;
    }

    double current_time() {
      return clock();
    }
};
