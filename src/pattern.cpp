#include "pattern.h"

using Token = char;

Pattern::Pattern(std::string pattern) : pattern(pattern) {
    auto vec = group_tokens(pattern);
    set_modified_atoms(vec);
    set_atoms(vec);
}

// helpers
bool is_start_bracket(char c) {
    return c == '[' || c == '(' || c == '{';
}

bool is_end_bracket(char c) {
    return c == ']' || c == ')' || c == '}';
}

bool is_modifier(char c) {
    return c == '+' || c == '*' || c == '?';
}

std::vector<std::string> Pattern::group_tokens(std::string pattern) {
    // splits the pattern into elements of groups
    /* example groups:
     *  a
     *  \d?
     *  [a-c]
     *  (a|bc)
     *  a+
    */

    std::string cur;
    std::vector<std::string> out;
    bool in_bracket;
    
    for (long unsigned int i = 0; i < pattern.size(); ++i) {
        char c = pattern[i];
        if (c == '\\') {
            continue;
        } else if (is_start_bracket(c)) {
            in_bracket = true;
        } else if (is_end_bracket(c)) {
            in_bracket = false;
        }
        
        if (!is_modifier(c)) {
            cur.push_back(c);
        } else if (!out.empty()) {
            // and c is a modifier
            out.back().push_back(c);
        }

        if (!in_bracket && !cur.empty()) {
            out.push_back(std::move(cur));
        }
    }
    return out;
}

void Pattern::set_modified_atoms(std::vector<std::string>& tokens) {
    // removes the modifier from all tokens that have it
    modifiers.reserve(tokens.size());
    for (long unsigned int i = 0; i < tokens.size(); ++i) {
        if (tokens[i].back() == '?') {
            tokens[i].pop_back();
            modifiers[i] = Modifier::OPTIONAL;
        } else if (tokens[i].back() == '*') {
            tokens[i].pop_back();
            modifiers[i] = Modifier::REPEATED_S;
        } else if (tokens[i].back() == '+') {
            tokens[i].pop_back();
            modifiers[i] = Modifier::REPEATED_P;
        }
    }
}

void Pattern::set_atoms(const std::vector<std::string>& tokens) {
    for (auto token : tokens) {
        if (token == "\w") {
            atoms.push_back(Atom::alphanumeric()); 
        } else if (token == "\W") {
            atoms.push_back(Atom::not_atom(Atom::alphanumeric()));
        } else if (token == "\d") {
            atoms.push_back(Atom::digit());
        } else if (token == "\D") {
            atoms.push_back(Atom::not_atom(Atom::digit()));
        } else if (token == "\s") {
            atoms.push_back(Atom::whitespace());
        } else if (token == "\S") {
            atoms.push_back(Atom::not_atom(Atom::whitespace()));
        } else if (token == ".") {
            atoms.push_back(Atom::character());
        } else if (token == "\.") {
            atoms.push_back(Atom {'.'});
        } else if (token == "\?") {
            atoms.push_back(Atom {'?'});
        } else if (token == "\$") {
            atoms.push_back(Atom {'$'});
        } else if (false) {
            // deal with ranges
        } else {
            // the token is just a char
            atoms.push_back(Atom {token[0]});
        }
    }
}

// todo snippets
// const reference
// vector
