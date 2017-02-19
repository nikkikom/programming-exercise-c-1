//
//  main.cpp
//  Path Normalization
//
//  Created by Nikki Chumakov on 19/02/2017.
//

#include <iostream>
#include "path_normalize.hpp"

int main (int argc, char const *argv[])
{
  if (argc == 1)
  {
    std::cerr << "Usage: normalize <path>...\n";
    return 1;
  }
  
  for (int i=1; i<argc; ++i)
    std::cout << (i>1 ? " " : "") << fs::normalize(argv[i]);
  
  std::cout << '\n';
  return 0;
}
