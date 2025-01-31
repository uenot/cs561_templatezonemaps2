#include <fstream>
#include <vector>
#include <iostream>
#include <random>
#include <set>
#include <algorithm>
#include <functional>
#include <chrono>
#include "zonemaps.h"
#include "zonemaps.cpp"

// Specify your path of workload file here
std::string kInputDataPath = "./workload.dat";
std::string kPointQueriesPath = "./point_queries.txt";
std::string kRangeQueriesPath = "./range_queries.txt";
const uint32_t kRuns = 3;
using namespace std;

void loadPointQueries(std::string & input_queries_path, std::vector<int> & queries)
{
  queries.clear();
  std::ifstream infile(input_queries_path, ios::in);
  int tmp;
  while (infile >> tmp) {
	  queries.push_back(tmp);
  }
  // shuffle indexes
  std::random_shuffle(queries.begin(), queries.end());
}

struct range_query {
  int start;
  int end;
};

// TODO
void loadRangeQueries(std::string & input_queries_path, std::vector<range_query> & queries)
{
  queries.clear();
  std::ifstream infile(input_queries_path, ios::in);
  int tmp;
  bool isStart = true;
  range_query rq;
  while (infile >> tmp) {
    if (isStart) {
      rq.start = tmp;
    } else {
      rq.end = tmp;
      queries.push_back(rq);
    }
    isStart = !isStart;
  }
  // shuffle indexes
  std::random_shuffle(queries.begin(), queries.end());

}

int main(int argc, char **argv)
{ 

  // read data
  std::ifstream ifs;
  std::vector<int> data;

  ifs.open(kInputDataPath, std::ios::binary);
  ifs.seekg(0, std::ios::end);
  size_t filesize = ifs.tellg();
  ifs.seekg(0, std::ios::beg);

  data.resize(filesize / sizeof(int));
  ifs.read((char *)data.data(), filesize);

  //1. ----------------------------- initialize zonemap and build -----------------------------
  //build zonemap
  zonemap<int> zones(data, (uint)data.size() / 100);

  //2. ----------------------------- point queries -----------------------------
  std::vector<int> queries;
  loadPointQueries(kPointQueriesPath, queries); 

  auto start_pq = std::chrono::high_resolution_clock::now();
  for (size_t r = 0; r < kRuns; r++) {
    for (int pq: queries) {
      // query from zonemaps here 
      zones.query(pq);
    }

    // shuffle indexes
    std::random_shuffle(queries.begin(), queries.end());
  }
  auto stop_pq = std::chrono::high_resolution_clock::now();
  auto duration_pq = std::chrono::duration_cast<std::chrono::microseconds>(stop_pq - start_pq);
  unsigned long long point_query_time = duration_pq.count();
  std::cout << "Time taken to perform point queries from zonemap = " << point_query_time*1.0/kRuns << " microseconds" << endl;
    
  //3. ----------------------------- range queries -----------------------------
  std::vector<range_query> range_queries;
  loadRangeQueries(kRangeQueriesPath, range_queries); 
  auto start_rq = std::chrono::high_resolution_clock::now();

  for (size_t r = 0; r < kRuns; r++) {
    for (range_query rq: range_queries) {
      // query from zonemaps here
      zones.query(rq.start, rq.end);
    }

    std::random_shuffle(range_queries.begin(), range_queries.end());
  }

  auto stop_rq = std::chrono::high_resolution_clock::now();
  auto duration_rq = std::chrono::duration_cast<std::chrono::microseconds>(stop_rq - start_rq);

  unsigned long long range_query_time = duration_rq.count();;

  std::cout << "Time taken to perform range query from zonemap = " << range_query_time*1.0/kRuns << " microseconds" << endl;
  return 0;
}
