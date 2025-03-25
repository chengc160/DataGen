#ifndef DATAGEN_H
#define DATAGEN_H

#include <map>
#include <string>
#include <vector>

#define FULL_MASK 0xffffffff

/**
 * Enum representing different data distributions
 */
enum class DATA_DISTRIBUTION { UNIFORM, GAUSSIAN, ZIPF };

/**
 * Enum representing supported data types
 */
enum class DATATYPE {
  VAL_SHORT,
  VAL_INT,
  VAL_UNSIGNED,
  VAL_SIZE_T,
  VAL_LONG,
  VAL_LONGLONG,
  VAL_FLOAT,
  VAL_DOUBLE
};

/**
 * Union to store different data types
 */
union VALUE {
  int int_val;
  long long_val;
  long long longlong_val;
  float float_val;
  double double_val;
};

/**
 * Class to generate synthetic data using different distributions
 */
class DataGen {
public:
  static const std::map<DATATYPE, std::string> INT_TYPE;
  static const std::map<DATATYPE, std::string> FLOATING_TYPE;

public:
  DataGen(size_t data_size, DATATYPE key_datatype, DATATYPE val_datatype);
  ~DataGen();

  bool generate(DATA_DISTRIBUTION data_distro, size_t num_groups = 0);
  bool writeResults(const std::string &file_path);
  static int getBits(const std::string &str);

private:
  bool dataGenerator(DATA_DISTRIBUTION data_distro, size_t num_groups);
  void typeConverter(DATATYPE data_type, VALUE &val, size_t val_gen);

  /**
   * Generates a Zipf (power law) distributed random variable
   * @param alpha Shape parameter
   * @param n Number of elements
   * @return A Zipf-distributed random variable
   */
  size_t zipf(double alpha, size_t n);

  /**
   * Multiplicative Linear Congruential Generator (LCG)
   * for generating uniform(0.0, 1.0) random numbers
   * @param seed Initial seed value
   * @return A random double between 0.0 and 1.0
   */
  double rand_val(int seed);

  /**
   * Generates a normally distributed random number
   * @param mu Mean
   * @param sigma Standard deviation
   * @return A normally distributed random number
   */
  double randn(double mu, double sigma);

private:
  size_t m_data_size;        // Total data size
  DATATYPE m_key_datatype;   // Data type of the key
  DATATYPE m_val_datatype;   // Data type of the value
  std::vector<size_t> m_oid; // Object ID storage
  std::vector<VALUE> m_key;  // Key values storage
  std::vector<VALUE> m_val;  // Value storage
};

#endif // DATAGEN_H
