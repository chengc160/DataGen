#include "DataGen.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>

const std::map<DATATYPE, std::string> DataGen::INT_TYPE = {
    {DATATYPE::VAL_SIZE_T, "size_t"},
    {DATATYPE::VAL_SHORT, "short"},
    {DATATYPE::VAL_INT, "int"},
    {DATATYPE::VAL_LONG, "long"},
    {DATATYPE::VAL_LONGLONG, "long long"},
    {DATATYPE::VAL_LONGDOUBLE, "long double"}};

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

bool DataGen::writeResults(const std::string &file_path, std::string &msg) {
  if (file_path.empty()) {
    msg = "Error: File path is empty!";
    return false;
  }

  std::ofstream out_file(file_path);
  if (out_file.is_open()) {
    out_file << m_data_size << " data points" << msg << std::endl;
    out_file << "OID\t Key\t Value" << std::endl;
    for (size_t i = 0; i < m_data_size; i++) {
      out_file << std::fixed << m_oid[i] << "\t" << variantToString(m_key[i])
               << "\t" << variantToString(m_val[i]) << std::endl;
    }
    msg = "Data are successfully written into " + file_path;
  } else {
    msg = "Unable to open file.";
  }

  // Placeholder: Implement file writing if needed

  return true;
}

bool DataGen::plotDistributionHistogram(int bins, int width) {
  if (m_data_size == 0) {
    return false;
  }

  std::vector<double> numericValues;
  for (const auto &v : m_key) {
    numericValues.push_back(variantToDouble(v));
  }

  if (numericValues.empty()) {
    return false;
  }

  double min = *std::min_element(numericValues.begin(), numericValues.end());
  double max = *std::max_element(numericValues.begin(), numericValues.end());
  double binSize = (max - min) / bins;

  std::vector<int> counts(bins, 0);
  for (const auto &val : numericValues) {
    int bin = std::min(static_cast<int>((val - min) / binSize), bins - 1);
    counts[bin]++;
  }

  int maxCount = *std::max_element(counts.begin(), counts.end());

  std::cout << "\nHistogram:\n";
  for (int i = 0; i < bins; ++i) {
    double binStart = min + i * binSize;
    double binEnd = binStart + binSize;

    // Print bucket index and bin range aligned
    std::cout << "Bucket[" << std::setw(2) << i << "] "
              << "[" << std::fixed << std::setw(10) << std::setprecision(0)
              << binStart << ", " << std::setw(10) << binEnd << ") ";

    // Compute and print the bar
    int barLength =
        static_cast<int>((static_cast<double>(counts[i]) / maxCount) * width);
    std::cout << std::left << std::string(barLength, '=') << std::right << " ("
              << counts[i] << ")" << std::endl;
  }

  return true;
}

int DataGen::getBits(const std::string &str) {
  const int BIT_PER_BYTE = 8;
  if (str == "short")
    return sizeof(short) * BIT_PER_BYTE;
  if (str == "int")
    return sizeof(int) * BIT_PER_BYTE;
  if (str == "unsigned")
    return sizeof(unsigned) * BIT_PER_BYTE;
  if (str == "size_t")
    return sizeof(size_t) * BIT_PER_BYTE;
  if (str == "long")
    return sizeof(long) * BIT_PER_BYTE;
  if (str == "long long")
    return sizeof(long long) * BIT_PER_BYTE;
  return 0;
}

bool DataGen::dataGenerator(DATA_DISTRIBUTION data_distro, size_t num_groups) {
  if (m_data_size == 0) {
    std::cerr << "Error: Data size is zero." << std::endl;
    return false;
  }

  // Random engine and uniform distribution
  int key_min = 0, key_max = RAND_MAX;
  double val_min = 0, val_max = RAND_MAX;
  std::random_device rd;  // Seed
  std::mt19937 gen(rd()); // Mersenne Twister engine
  std::uniform_real_distribution<> dist_key(key_min, key_max);
  std::uniform_real_distribution<> dist_val(val_min, val_max);

  switch (data_distro) {
  case DATA_DISTRIBUTION::UNIFORM:
    std::cout << "Generating " << getBits(INT_TYPE.at(m_key_datatype))
              << "-bit keys from UNIFORM distribution...\n";

    for (size_t i = 0; i < m_data_size; i++) {
      m_oid[i] = i;
      m_key[i] = dist_key(gen);
      m_val[i] = dist_val(gen);
    }
    break;

  case DATA_DISTRIBUTION::GAUSSIAN:
    std::cout << "Generating " << getBits(INT_TYPE.at(m_key_datatype))
              << "-bit keys from GAUSSIAN distribution...\n";
    for (size_t i = 0; i < m_data_size; i++) {
      m_oid[i] = i;
      m_key[i] = randn(FULL_MASK / 2, pow(2, 29));
      m_val[i] = std::rand();
    }
    break;

  case DATA_DISTRIBUTION::ZIPF:
    std::cout << "Generating " << getBits(INT_TYPE.at(m_key_datatype))
              << "-bit keys from ZIPF distribution...\n";
    for (size_t i = 0; i < m_data_size; i++) {
      m_oid[i] = i;
      m_key[i] = static_cast<long>(zipf(1.0, m_data_size));
      m_val[i] = std::rand();
    }
    break;

  default:
    std::cerr << "Error: Undefined data distribution!" << std::endl;
    break;
  }

  return true;
}

double DataGen::variantToDouble(const VALUE &v) const {
  return std::visit([](auto &&v) -> double { return static_cast<double>(v); },
                    v);
}

int DataGen::variantToInt(const VALUE &v) const {
  return std::visit([](auto &&v) -> int { return static_cast<int>(v); }, v);
}

std::string DataGen::variantToString(const VALUE &v) const {
  return std::visit(
      [](const auto &val) -> std::string {
        std::ostringstream oss;
        oss << val;
        return oss.str();
      },
      v);
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
