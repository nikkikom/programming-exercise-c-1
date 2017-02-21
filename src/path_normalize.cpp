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
  
  size_type result_length = length;
  if (path[0] != delim) ++result_length;
  
  std::string result (result_length, '\0');

  // find end of domain part
  auto path_start = path.find (delim);
  if (path_start == std::string::npos) path_start = length;
  
  std::memcpy (&result[0], &path[0], path_start * sizeof (char));
  
  auto result_idx = path_start;
  
  // tokenize
  for (auto start=path_start, end=path_start; end<length; start=end)
  {
    end = path.find (delim, start+1);
    if (end == std::string::npos) end = length;
    
    size_type len = end-start;
    
    // skipping empty parts and '/.'
    if (len <= 1) continue;
    if (len == 2 && path[start+1] == C_DOT) continue;
    
    // copy every part but '/..'
    if (len != 3 || path[start+1] != C_DOT || path[start+2] != C_DOT)
    {
      std::memcpy (&result[result_idx], &path[start], len * sizeof (char));
      result_idx += len;
    }
    // parent entry inhibits the last part
    else
    {
      while (result_idx > path_start && result[--result_idx] != delim);
    }
  }

  // add trailing slash if there are no parts
  if (result_idx <= path_start) result[result_idx++] = delim;
  
  // shrink result
  result.resize (result_idx);
  
  return result;
}

} // namespace fs
