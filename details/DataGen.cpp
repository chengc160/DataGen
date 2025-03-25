#include "DataGen.h"
#include <assert.h>
#include <iostream>
#include <math.h>
#include <stdlib.h>

#define FALSE 0 // Boolean false
#define TRUE 1  // Boolean true

DataGen::DataGen(size_t data_size, DATATYPE key_datatype, DATATYPE val_datatype)
    : m_data_size(data_size), m_key_datatype(key_datatype),
      m_val_datatype(val_datatype) {
  m_oid.reserve(data_size);
  m_key.reserve(data_size);
  m_val.reserve(data_size);
}

DataGen::~DataGen() {}

bool DataGen::generate(DATA_DISTRIBUTION data_distro,
                       size_t group_nums /*=0*/) {

  return dataGenerator(data_distro, group_nums);
}

bool DataGen::writeResults(const std::string &file_path) {
  if (file_path.empty()) {
    std::cerr << "given file path is empty!" << std::endl;
    return false;
  }
  for (size_t i = 0; i < m_data_size; i++) {
    std::cout << "OID: " << m_oid[i] << " Key: " << m_key[i].longlong_val
              << "Val: " << m_val[i].double_val << std::endl;
  }

  return true;
}

bool DataGen::dataGenerator(DATA_DISTRIBUTION data_distro, size_t group_nums) {
  if (m_data_size == 0) {
    return false;
  }

  switch (data_distro) {
  case UNIFORM:
    for (size_t i = 0; i < m_data_size; i++) {
      m_oid[i] = i;
      if (group_nums == 0) {
        typeConverter(m_key_datatype, m_key[i], (unsigned)rand());
      } else {
        typeConverter(m_key_datatype, m_key[i], (unsigned)rand() % group_nums);
      }
      typeConverter(m_val_datatype, m_val[i], rand());
    }
    break;
  case GAUSSIAN:
    for (size_t i = 0; i < m_data_size; i++) {
      m_oid[i] = i;
      typeConverter(m_key_datatype, m_key[i], randn(FULL_MASK / 2, pow(2, 29)));
      typeConverter(m_val_datatype, m_val[i], rand());
    }
    break;
  case ZIPF:
    for (size_t i = 0; i < m_data_size; i++) {
      m_oid[i] = i;
      typeConverter(m_key_datatype, m_key[i], zipf(1.0, pow(2, 16)));
      typeConverter(m_val_datatype, m_val[i], rand());
    }
    break;

  default:
    std::cerr << "undefined data distribution!" << std::endl;
    break;
  }

  return true;
}

void DataGen::typeConverter(DATATYPE data_type, VALUE &val, size_t val_gen) {
  switch (data_type) {
  case VAL_INT:
    val.int_val = static_cast<int>(val_gen);
    break;
  case VAL_LONG:
    val.long_val = static_cast<long>(val_gen);
    break;
  case VAL_LONGLONG:
    val.longlong_val = static_cast<long long>(val_gen);
    break;
  case VAL_FLOAT:
    val.float_val = static_cast<float>(val_gen);
    break;
  case VAL_DOUBLE:
    val.double_val = static_cast<double>(val_gen);

  default:
    break;
  }
}

size_t DataGen::zipf(double alpha, size_t n) {
  static int first = TRUE; // Static first time flag
  static double c = 0;     // Normalization constant
  double z;                // Uniform random number (0 < z < 1)
  double sum_prob;         // Sum of probabilities
  double zipf_value;       // Computed exponential value to be returned
  int i;                   // Loop counter

  // Compute normalization constant on first call only
  if (first == TRUE) {
    for (i = 1; i <= n; i++)
      c = c + (1.0 / pow((double)i, alpha));
    c = 1.0 / c;
    first = FALSE;
  }

  // Pull a uniform random number (0 < z < 1)
  do {
    z = rand_val(0);
  } while ((z == 0) || (z == 1));

  // Map z to the value
  sum_prob = 0;
  for (i = 1; i <= n; i++) {
    sum_prob = sum_prob + c / pow((double)i, alpha);
    if (sum_prob >= z) {
      zipf_value = i;
      break;
    }
  }

  // Assert that zipf_value is between 1 and N
  assert((zipf_value >= 1) && (zipf_value <= n));

  return (zipf_value);
}

double DataGen::rand_val(int seed) {
  const long a = 16807;      // Multiplier
  const long m = 2147483647; // Modulus
  const long q = 127773;     // m div a
  const long r = 2836;       // m mod a
  static long x;             // Random int value
  long x_div_q;              // x divided by q
  long x_mod_q;              // x modulo q
  long x_new;                // New x value

  // Set the seed if argument is non-zero and then return zero
  if (seed > 0) {
    x = seed;
    return (0.0);
  }

  // RNG using integer arithmetic
  x_div_q = x / q;
  x_mod_q = x % q;
  x_new = (a * x_mod_q) - (r * x_div_q);
  if (x_new > 0)
    x = x_new;
  else
    x = x_new + m;

  // Return a random value between 0.0 and 1.0
  return ((double)x / m);
}

double DataGen::randn(double mu, double sigma) {
  double U1, U2, W, mult;
  static double X1, X2;
  static int call = 0;

  if (call == 1) {
    call = !call;
    return (mu + sigma * (double)X2);
  }

  do {
    U1 = -1 + ((double)rand() / RAND_MAX) * 2;
    U2 = -1 + ((double)rand() / RAND_MAX) * 2;
    W = pow(U1, 2) + pow(U2, 2);
  } while (W >= 1 || W == 0);

  mult = sqrt((-2 * log(W)) / W);
  X1 = U1 * mult;
  X2 = U2 * mult;

  call = !call;

  return (mu + sigma * (double)X1);
}
