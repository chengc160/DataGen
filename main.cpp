#include "DataGen.h"
#include <assert.h>
#include <iostream>

int main(int argc, char *argv[]) {
  const size_t data_size = 1000000;
  std::cout << "synthetic data gen.." << std::endl;

  const std::string output_file = "data.txt";

  DataGen data_gen(data_size, DATATYPE::VAL_LONGLONG, DATATYPE::VAL_DOUBLE);
  data_gen.generate(DATA_DISTRIBUTION::UNIFORM);
  data_gen.writeResults(output_file);

  return 0;
}