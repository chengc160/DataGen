#include "DataGen.h"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>

const std::map<DATATYPE, std::string> DataGen::INT_TYPE = {
    {DATATYPE::VAL_SHORT, "short"},
    {DATATYPE::VAL_INT, "int"},
    {DATATYPE::VAL_UNSIGNED, "unsigned"},
    {DATATYPE::VAL_SIZE_T, "size_t"},
    {DATATYPE::VAL_LONG, "long"},
    {DATATYPE::VAL_LONGLONG, "long long"}};

const std::map<DATATYPE, std::string> DataGen::FLOATING_TYPE = {
    {DATATYPE::VAL_FLOAT, "float"}, {DATATYPE::VAL_DOUBLE, "double"}};

DataGen::DataGen(size_t data_size, DATATYPE key_datatype, DATATYPE val_datatype)
    : m_data_size(data_size), m_key_datatype(key_datatype),
      m_val_datatype(val_datatype) {
  m_oid.resize(data_size);
  m_key.resize(data_size);
  m_val.resize(data_size);
}

DataGen::~DataGen() {}

bool DataGen::generate(DATA_DISTRIBUTION data_distro, size_t num_groups) {
  return dataGenerator(data_distro, num_groups);
}

bool DataGen::writeResults(const std::string &file_path) {
  if (file_path.empty()) {
    std::cerr << "Error: File path is empty!" << std::endl;
    return false;
  }

  // Placeholder: Implement file writing if needed

  return true;
}

int DataGen::getBits(const std::string &str) {
  const int bits_per_byte = 8;
  if (str == "short")
    return sizeof(short) * bits_per_byte;
  if (str == "int")
    return sizeof(int) * bits_per_byte;
  if (str == "unsigned")
    return sizeof(unsigned) * bits_per_byte;
  if (str == "size_t")
    return sizeof(size_t) * bits_per_byte;
  if (str == "long")
    return sizeof(long) * bits_per_byte;
  if (str == "long long")
    return sizeof(long long) * bits_per_byte;
  return 0;
}

bool DataGen::dataGenerator(DATA_DISTRIBUTION data_distro, size_t num_groups) {
  if (m_data_size == 0) {
    std::cerr << "Error: Data size is zero." << std::endl;
    return false;
  }

  switch (data_distro) {
  case DATA_DISTRIBUTION::UNIFORM:
    std::cout << "Generating " << getBits(INT_TYPE.at(m_key_datatype))
              << "-bit keys from UNIFORM distribution...\n";
    for (size_t i = 0; i < m_data_size; i++) {
      m_oid[i] = i;
      typeConverter(m_key_datatype, m_key[i],
                    num_groups == 0 ? rand() : rand() % num_groups);
      typeConverter(m_val_datatype, m_val[i], rand());
    }
    break;

  case DATA_DISTRIBUTION::GAUSSIAN:
    std::cout << "Generating " << getBits(INT_TYPE.at(m_key_datatype))
              << "-bit keys from GAUSSIAN distribution...\n";
    for (size_t i = 0; i < m_data_size; i++) {
      m_oid[i] = i;
      typeConverter(m_key_datatype, m_key[i], randn(FULL_MASK / 2, pow(2, 29)));
      typeConverter(m_val_datatype, m_val[i], rand());
    }
    break;

  case DATA_DISTRIBUTION::ZIPF:
    std::cout << "Generating " << getBits(INT_TYPE.at(m_key_datatype))
              << "-bit keys from ZIPF distribution...\n";
    for (size_t i = 0; i < m_data_size; i++) {
      m_oid[i] = i;
      typeConverter(m_key_datatype, m_key[i], zipf(1.0, m_data_size));
      typeConverter(m_val_datatype, m_val[i], rand());
    }
    break;

  default:
    std::cerr << "Error: Undefined data distribution!" << std::endl;
    return false;
  }

  return true;
}

void DataGen::typeConverter(DATATYPE data_type, VALUE &val, size_t val_gen) {
  switch (data_type) {
  case DATATYPE::VAL_INT:
    val.int_val = static_cast<int>(val_gen);
    break;
  case DATATYPE::VAL_LONG:
    val.long_val = static_cast<long>(val_gen);
    break;
  case DATATYPE::VAL_LONGLONG:
    val.longlong_val = static_cast<long long>(val_gen);
    break;
  case DATATYPE::VAL_FLOAT:
    val.float_val = static_cast<float>(val_gen);
    break;
  case DATATYPE::VAL_DOUBLE:
    val.double_val = static_cast<double>(val_gen);
    break;
  }
}

size_t DataGen::zipf(double alpha, size_t n) {
  static bool first = true;
  static double c = 0;
  double z, sum_prob;
  size_t zipf_value = 1;

  if (first) {
    for (size_t i = 1; i <= n; i++)
      c += (1.0 / pow(i, alpha));
    c = 1.0 / c;
    first = false;
  }

  do {
    z = rand_val(0);
  } while (z == 0 || z == 1);

  sum_prob = 0;
  for (size_t i = 1; i <= n; i++) {
    sum_prob += c / pow(i, alpha);
    if (sum_prob >= z) {
      zipf_value = i;
      break;
    }
  }

  assert(zipf_value >= 1 && zipf_value <= n);
  return zipf_value;
}

double DataGen::rand_val(int seed) {
  static long x = 1;
  const long a = 16807, m = 2147483647, q = 127773, r = 2836;

  if (seed > 0) {
    x = seed;
    return 0.0;
  }

  long x_new = (a * (x % q)) - (r * (x / q));
  x = (x_new > 0) ? x_new : x_new + m;
  return static_cast<double>(x) / m;
}

double DataGen::randn(double mu, double sigma) {
  static bool call = false;
  static double X2;

  if (call) {
    call = false;
    return mu + sigma * X2;
  }

  double U1, U2, W;
  do {
    U1 = -1 + 2 * ((double)rand() / RAND_MAX);
    U2 = -1 + 2 * ((double)rand() / RAND_MAX);
    W = U1 * U1 + U2 * U2;
  } while (W >= 1 || W == 0);

  double mult = sqrt(-2 * log(W) / W);
  X2 = U2 * mult;
  call = true;

  return mu + sigma * (U1 * mult);
}
