#include "pattern.h"

using Token = char;

Pattern::Pattern(std::string pattern) : pattern(pattern) {
    auto grouped = group_tokens(pattern);
    auto vec = extend_repeated_tokens(grouped);
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
    modifiers.resize(tokens.size());
    for (long unsigned int i = 0; i < tokens.size(); ++i) {
        if (tokens[i].back() == '?') {
            tokens[i].pop_back();
            modifiers[i] = Modifier::OPTIONAL;
        } else if (tokens[i].back() == '*') {
            tokens[i].pop_back();
            modifiers[i] = Modifier::REPEATED;
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
        } else if (token == "\^") {
            atoms.push_back(Atom {'^'});
        } else if (token == "\\") {
            atoms.push_back(Atom {'\\'});
        } else if (false) {
            // deal with ranges
        } else {
            // the token is just a char
            atoms.push_back(Atom {token[0]});
        }
    }
}

std::vector<std::string> Pattern::extend_repeated_tokens(
        const std::vector<std::string>& tokens) {
    // change all x+ to xx*
    // repeat all {m,n} repetitions: x{m,n} = x{m}[x]{n-m}
    std::vector<std::string> out;
    for (auto token : tokens) {
        if (token.back() == '+') {
            // make a copy of token without the +
            auto cp = token;
            cp.pop_back();
            // add cp and cp*
            out.push_back(cp);
            out.push_back(cp + "*");
        } else if (token.front() == '{' && token.back() == '}' && !out.empty()) {
            // if it has 2 indices
            auto index = token.find(",");
            if (index != std::string::npos) {
                int m = std::stoi(token.substr(1, index));
                int n = std::stoi(token.substr(index + 1, token.size() - 1));
                // make m copies of the last item (loop m-1 times)
                for (int i = 1; i < m; ++i) {
                    out.push_back(out.back());
                }
                // make n-m copies of the last item + ? (loop n-m times)
                auto last = out.back() + "?";
                for (int i = 0; i < n-m; ++i) {
                    out.push_back(last);
                }
            }
            // if it has 1 index
            // the number would be the enitre substring inside brackets
            int num = std::stoi(token.substr(1, token.size() - 2));
            // make num copies of the last item (loop num-1 times)
            for (int i = 1; i < num; ++i) {
                out.push_back(out.back());
            }
        } else {
            out.push_back(token);
        }
    }
    return out;
}
