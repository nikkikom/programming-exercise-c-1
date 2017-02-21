//
//  main.cpp
//  Path Normalization
//
//  Created by Nikki Chumakov on 19/02/2017.
//

#include <iostream>
#include "path_normalize.hpp"

bool test (std::string const& p, std::string const& should)
{
  std::string r = fs::normalize (p);
  bool success = (should == r);
  
  if (success) std::cout << "ok   ";
  else std::cout << "fail ";

  std::cout << p << " -> " << r;
  if (!success) std::cout << " (should be " << should << ")";
  std::cout << "\n";
  return success;
}

int unit_tests ()
{
  int failed = 0;
  failed += !test ("Y/AAA/../CCC/./DDD/../EEE/../FFF/./../..", "Y/");
  failed += !test ("/", "/");
  failed += !test ("//", "/");
  failed += !test ("///", "/");
  failed += !test ("f", "f/");
  failed += !test ("foo", "foo/");
  failed += !test ("foo/", "foo/");
  failed += !test ("foo/.", "foo/");
  failed += !test ("/foo", "/foo");
  failed += !test ("/foo/", "/foo");
  failed += !test ("/foo/.", "/foo");
  failed += !test ("/foo/bar", "/foo/bar");
  failed += !test ("..", "../");
  failed += !test ("../..", "../");
  failed += !test ("../../.", "../");
  failed += !test ("/..", "/");
  failed += !test ("/../..", "/");
  failed += !test ("/foo/../../../../bar/./../foo/../../../bar/./foo/./bar/../..", "/bar");
  failed += !test ("../foo", "../foo");
  failed += !test (".", "./");
  failed += !test ("./.", "./");
  failed += !test ("/foo/..", "/");
  failed += !test ("/foo/...", "/foo/...");
  failed += !test ("/foo/.../.", "/foo/...");
  failed += !test ("/foo/..bar", "/foo/..bar");
  failed += !test ("../f", "../f");
  failed += !test ("/../f", "/f");
  failed += !test ("/net/foo/bar", "/net/foo/bar");
  failed += !test ("/net/foo/bar/.", "/net/foo/bar");
  failed += !test ("net/foo/bar/", "net/foo/bar");
  return failed;
}

int main(int argc, const char * argv[])
{
  if (argc < 2)
    return unit_tests();
  
  int failed = 0;
  for (int i=1; i<argc-1; i += 2)
    failed += !test (argv[i], argv[i+1]);
  
  return failed;
}
