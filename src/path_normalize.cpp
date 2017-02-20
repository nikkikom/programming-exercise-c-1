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
  
#define C_DOT '.'
  
// Assumptions:
// 1. no validity check for domain name. everything before the first slash
//    counted as domain name part.
// 2. may be rewritten to wide string very easy. string should be
//    changed to wstring, char -> wchar_t, S_* and C_SLASH constants should
//    be written as wide strings or wide chars.
std::string
normalize(std::string const& path, char delim)
{
  using size_type = std::string::size_type;
  auto const length = path.length ();

  // store <start pos, length> of every path part.
  // parts include leading slashes.
  typedef std::pair<size_type, size_type> position;
  std::vector<position> parts;
  
  // tokenize
  auto saved_start = path.find (delim);

  for (auto start=saved_start, end=saved_start; end<length; start=end)
  {
    end = path.find (delim, start+1);
    if (end == std::string::npos) end = length;
    
    // skipping empty parts and '/.'
    if (end-start <= 1 || (end-start == 2 && path[start+1] == C_DOT))
    continue;
    
    // register everything but '/..' entries as path part
    if (end-start != 3 || path[start+1] != C_DOT || path[start+2] != C_DOT)
    {
      parts.emplace_back (start, end-start);
    }
    // parent entry inhibits the last registered part
    else if (! parts.empty ())
    {
      parts.pop_back ();
    }
  }
  
  // no parts found -> return just domain name and trailing slash
  if (parts.empty ())
    return path.substr (0, saved_start) + delim;

  // result length cannot be greater than input path length
  std::string ret (length, 0);
  
  // copy domain part
  std::memcpy (&ret[0], &path[0], saved_start);
  
  // copy each part into result string
  auto s = &ret[saved_start];
  for (auto const& part: parts)
  {
    std::memcpy(s, &path[part.first], part.second * sizeof (char));
    s += part.second;
  }
  
  // shrink result string
  ret.resize (size_type (s - &ret[0]));
  
  return ret;
}

} // namespace fs
