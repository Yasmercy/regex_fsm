#ifndef REGEX_H
#define REGEX_H

#include <vector>
#include <string>
#include "result.h"
#include "pattern.h"
#include "machine_factory.h"

class RegEx {
    // class for regular expression matching
public:
    // constructors
    RegEx(std::string pattern);
    
    // methods
    Result match(std::string expr);
    nlohmann::json get_machine_json();

private:
    Pattern pattern;
    StateMachine m;
};

#endif
