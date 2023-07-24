#include "atom.h"
#include <algorithm>
#include <iterator>

using Token = char;

Atom::Atom(Token start_inclusive, Token end_inclusive) {
    insert_range(start_inclusive, ++end_inclusive);
}

Atom::Atom(Token token) {
    tokens.insert(token);
}

Atom Atom::alphanumeric() {
    Atom a = digit();
    a.insert_range('A', 'Z' + 1); // A-Z
    a.insert_range('a', 'z' + 1); // a-z
    return a;
}

Atom Atom::digit() {
    return Atom {'0', '9'}; // 0-9
}

Atom Atom::character() {
    return Atom {'\1'};
}

Atom Atom::whitespace() {
    Atom a {' '}; 
    a.tokens.insert('\t');
    a.tokens.insert('\n');
    a.tokens.insert('\r');
    return a;
}

Atom Atom::not_atom(const Atom& other) {
    const Atom a {' ', '~'};
    Atom out;
    std::set_difference(
        a.tokens.begin(), a.tokens.end(),
        other.tokens.begin(), other.tokens.end(),
        std::inserter(out.tokens, out.tokens.begin())
    );
    return out;
}

void Atom::append(const Atom& other) {
    std::copy(other.tokens.begin(), other.tokens.end(), std::inserter(tokens, tokens.begin()));
}

void Atom::insert_range(Token start, Token end) {
    for (Token s = start; s < end; ++s) {
        tokens.insert(s);
    }
}

std::set<Token> Atom::get_tokens() const {
    return tokens;
}
