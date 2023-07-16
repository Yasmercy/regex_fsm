#ifndef REGEX_H
#define REGEX_H

#include <vector>
#include <string>
#include "result.h"
#include "pattern.h"

class RegEx {
    // class for regular expression matching
public:
    // constructors
    RegEx(std::string pattern);
    
    // methods
    Result match(std::string expr);

private:
    Pattern pattern;
};

#endif
