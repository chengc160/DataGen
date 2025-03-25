#ifndef DATAGEN_H
#define DATAGEN_H

#include <string>
#include <vector>

#define FULL_MASK 0xffffffff

enum DATA_DISTRIBUTION {
  UNIFORM,
  GAUSSIAN,
  ZIPF,
};

enum DATATYPE { VAL_INT, VAL_LONG, VAL_LONGLONG, VAL_FLOAT, VAL_DOUBLE };

union VALUE {
  int int_val;
  long long_val;
  long long longlong_val;
  float float_val;
  double double_val;
};

class DataGen {
public:
  DataGen(size_t data_size, DATATYPE m_key_data_type, DATATYPE val_data_type);
  ~DataGen();

  bool generate(DATA_DISTRIBUTION data_distro, size_t group_nums = 0);
  bool writeResults(const std::string &file_path);

private:
  bool dataGenerator(DATA_DISTRIBUTION data_distro, size_t group_nums);
  void typeConverter(DATATYPE data_type, VALUE &val, size_t val_gen);
  /**
   * Function to generate Zipf (power law) distributed random variables
   * Input: alpha and N
   * Output: Returns with Zipf distributed random variable
   */
  size_t zipf(double alpha, size_t n);
  /**
   * Multiplicative LCG for generating uniform(0.0, 1.0) random numbers
   * x_n = 7^5*x_(n-1)mod(2^31 - 1)
   * With x seeded to 1 the 10000th x value should be 1043618065
   * From R. Jain, "The Art of Computer Systems Performance Analysis,"
   * John Wiley & Sons, 1991. (Page 443, Figure 26.2)
   */
  double rand_val(int seed);
  double randn(double mu, double sigma);

private:
  /* data */
  size_t m_data_size;
  DATATYPE m_key_datatype;
  DATATYPE m_val_datatype;
  std::vector<size_t> m_oid;
  std::vector<VALUE> m_key;
  std::vector<VALUE> m_val;
};

#endif