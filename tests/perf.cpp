//
//  main.cpp
//  Path Normalization
//
//  Created by Nikki Chumakov on 19/02/2017.
//

#include <iostream>
#include "path_normalize.hpp"

// Seconds for each test
#define SECS_PER_TEST 10

// Max path size
#define MAX_PATH_SIZE 100000

void perf (std::size_t len, time_t t)
{
  // some test pattern
  char const pat[] = "/foo/bar/../eee/gg/.//h/.ft/../../.../com/far/./meo";
  std::string path = "domain.com";
  while (path.length () < len)
    path += pat;
  
  path.resize(len);

  std::string result;
  unsigned long long iters = 0;
  for (time_t end = time (0) + t; end > time (0); ++iters)
    result = fs::normalize(path);
  
  
  std::cout << iters/(unsigned long long)t << " calls/s, ";
  
  auto perf = iters * len / (unsigned long long)t;
  char const* dim = "bytes/s";
  
  if (perf >= 1024)
  {
    perf /= 1024;
    dim = "KB/s";
  }
  
  if (perf >= 1024)
  {
    perf /= 1024;
    dim = "MB/s";
  }
  
  if (perf >= 1024)
  {
    perf /= 1024;
    dim = "GB/s";
  }
  
  std::cout << perf  << ' ' << dim << '\n';
}

int main(int argc, const char * argv[])
{
  if (argc < 2 || argc > 3)
  {
    std::cerr << "Usage: perf <length> <seconds>\n";
    return 1;
  }
  
  std::size_t path_size = MAX_PATH_SIZE;
  int seconds = SECS_PER_TEST;
  
  if (argc > 1) path_size = (size_t) atoll (argv[1]);
  if (argc > 2) seconds = atoi (argv[2]);
  
  std::cout << "path length=" << path_size << " ";
  perf (path_size, seconds);
  return 0;
}
