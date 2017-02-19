//
//  path_normalize.cpp
//  Path Normalization
//
//  Created by Nikki Chumakov on 19/02/2017.
//

#include "path_normalize.hpp"

#include <vector>
#include <cstring>

namespace fs {
  
#define S_CURRENT "."
#define S_PARENT ".."
  
// Assumptions:
// 1. no validity check for domain name. everything before the first slash
//    counted as domain name part.
// 2. may be rewritten to wide string very easy. string should be
//    changed to wstring, char -> wchar_t, S_* and C_SLASH constants should
//    be written as wide strings or wide chars.
std::string
normalize(std::string path, char delim)
{
  using size_type = std::string::size_type;
  auto const length = path.length ();

  // will store <start pos, length> of every path part. parts includes
  // leading slashes.
  typedef std::pair<size_type, size_type> position;
  std::vector<position> parts;
  
  // tokenize
  auto saved_start = path.find (delim);
  for (auto start=saved_start, end=saved_start; end<length; start=end)
  {
    end = path.find (delim, start+1);
    if (end == std::string::npos) end = length;
   
    // skipping empty parts and '.'
    if (end-start <= 1 || path.substr (start+1, end-start-1) == S_CURRENT)
      continue;

    // register everything but 'parent' entries as path part
    if (path.substr (start+1, end-start-1) != S_PARENT)
      parts.emplace_back (start, end-start);
    // parent entry enhibit the last registered path part
    else if (! parts.empty ())
      parts.pop_back ();
  }
  
  // no parts found -> pring domain name and trailing slash
  if (parts.empty ())
    return path.substr (0, saved_start) + delim;

  auto s = &path[saved_start];
  
  // shift every part in-place, eliminating gaps and creating continous string.
  for (auto const& part: parts)
  {
    std::memmove(s, &path[part.first], part.second * sizeof (char));
    s += part.second;
  }
  
  // strip unused tail that we have now because of shifting
  path.resize ((size_t) (s - &path[0]));
  
  return path;
}
}
