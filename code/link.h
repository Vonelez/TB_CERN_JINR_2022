#pragma once
#include <vector>
#include <string>
#include "apv_claster.h"
namespace {
  std::vector<int> vector_int;
  std::vector<std::vector<int>> vector_vector_int;
  std::vector<unsigned int> vector_uint;
  std::vector<std::vector<unsigned int>> vector_vector_uint;

  std::vector<short> vector_short;
  std::vector<std::vector<short>> vector_vector_short;
  std::vector<char> vector_char;
  std::vector<std::vector<char>> vector_vector_char;  
  
  std::vector<double> vector_double;
  std::vector<std::vector<double>> vector_vector_double;
  std::vector<float> vector_float;
  std::vector<std::vector<float>> vector_vector_float;

  std::vector<std::string> vector_string;
  std::vector<std::vector<std::string>> vector_vector_string;

  std::vector<apvHit> vector_apvHit;
  std::vector<std::vector<apvHit>> vector_vector_apvHit;
  std::vector<apvClaster> vector_apvClaster;
  std::vector<std::vector<apvClaster>> vector_vector_apvClaster;
  std::vector<apvTrack> vector_apvTrack;
  std::vector<std::vector<apvTrack>> vector_vector_apvTrack;

};