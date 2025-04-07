#include "DataGen.h"
#include <cassert>
#include <iomanip>
#include <iostream>

void printSystemMetrics() {
  std::cout << std::left;

  // Print data types
  for (const auto &t : DataGen::INT_TYPE) {
    std::cout << std::setw(10) << t.second;
  }
  std::cout << std::endl;

  // Print corresponding bit sizes
  for (const auto &t : DataGen::INT_TYPE) {
    std::cout << std::setw(10) << DataGen::getBits(t.second);
  }
  std::cout << std::endl;
}

int main(int argc, char *argv[]) {
  printSystemMetrics();

  // Argument parsing with safety checks
  const size_t data_size = (argc > 1) ? std::atoi(argv[1]) : 1'000'000;
  const size_t num_groups = (argc > 2) ? std::atoi(argv[2]) : 0;
  const DATA_DISTRIBUTION data_distribution =
      (argc > 3) ? static_cast<DATA_DISTRIBUTION>(std::atoi(argv[3]))
                 : DATA_DISTRIBUTION::UNIFORM;
  const std::string output_file = (argc > 4) ? argv[4] : "./data.txt";

  std::cout << "Synthetic data generation:\n";
  std::string msg;
  // Initialize data generator
  DataGen data_gen(data_size, DATATYPE::VAL_LONGLONG, DATATYPE::VAL_DOUBLE);

  // Generate and write results
  data_gen.generate(data_distribution, num_groups);
  data_gen.plotDistributionHistogram(20);
  data_gen.writeResults(output_file, msg);
  std::cout << msg << std::endl;

  return 0;
}