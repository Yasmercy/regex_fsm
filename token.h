#ifndef TOKEN_H
#define TOKEN_H

#include <variant>
#include <string>

struct LeftParen {};

struct RightParen {};

struct Alternate {};

struct KleeneStar {};

struct Concatenate {};

using Token = std::variant<LeftParen, RightParen, Alternate, KleeneStar, Concatenate, char>;

#endif
