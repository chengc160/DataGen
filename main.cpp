#include "DataGen.h"
#include <assert.h>
#include <iostream>

int main(int argc, char *argv[]) {
  const size_t data_size = 1000000;
  std::cout << "synthetic data gen.." << std::endl;

  const std::string output_file = "data.txt";
  VALUE_DATATYPE double_type = VALUE_DATATYPE::VAL_DOUBLE;

  // DataGen data_gen(data_size, double_type);
  // data_gen.generate(DATA_DISTRIBUTION::UNIFORM);
  // data_gen.writeResults(output_file);

  return 0;
}