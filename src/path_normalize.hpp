//
//  path_normalize.hpp
//  Path Normalization
//
//  Created by Nikki Chumakov on 19/02/2017.
//

#ifndef path_normalize_hpp
#define path_normalize_hpp
#include <string>


namespace fs {
  
char const C_SLASH = '/';

std::string normalize(std::string path, char delim = C_SLASH);

}

#endif /* path_normalize_hpp */
