#ifndef PATTERN_H
#define PATTERN_H

#include <string>
#include <vector>
#include "atom.h"

enum Modifier { 
    NONE,
    OPTIONAL, // ?
    REPEATED, // *
};

struct Pattern {
    // constructor
    Pattern(std::string pattern);
    
    // helper functions
    static std::vector<std::string> group_tokens(std::string pattern);
    static std::vector<std::string> extend_repeated_tokens(const std::vector<std::string>& tokens);
    void set_modified_atoms(std::vector<std::string>& tokens);
    void set_atoms(const std::vector<std::string>& tokens);

    // public members
    std::string pattern;
    std::vector<Atom> atoms;
    std::vector<Modifier> modifiers;
};


#endif
