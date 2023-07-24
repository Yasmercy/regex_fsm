#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <vector>
#include "atom.h"

enum Modifier { 
    OPTIONAL,   // ?
    REPEATED_S, // *
    REPEATED_P, // +
    NONE
};

class Pattern {
public:
    Pattern(std::string pattern);
    std::string pattern;
    
    // helper functions
    static std::vector<std::string> group_tokens(std::string pattern);
    void set_modified_atoms(std::vector<std::string>& tokens);
    void set_atoms(const std::vector<std::string>& tokens);
private:
    std::vector<Atom> atoms;
    std::vector<Modifier> modifiers;

};


#endif
