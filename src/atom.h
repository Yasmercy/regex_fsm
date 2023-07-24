#ifndef ATOM_H
#define ATOM_H

#include <set>

class Atom {
    using Token = char;
    // this is the smallest building block of a regex pattern
    // this is essentially a vector of characters
public:
    Atom(Token start_inclusive, Token end_inclusive);
    Atom(Token token);
    
    // various builder methods
    static Atom alphanumeric();
    static Atom digit();
    static Atom character();
    static Atom whitespace();
    static Atom not_atom(const Atom& other);
    
    // modifier
    void append(const Atom& other);

    // getter
    std::set<Token> get_tokens() const;
private:
    Atom() = default;
    std::set<Token> tokens;
    void insert_range(Token start, Token end);
};

#endif
