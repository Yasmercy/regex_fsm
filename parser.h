#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <string>
#include "token.h"

// checks if parens are all matched
// check if all operations have the correct operands
bool valid(const std::vector<Token>& tokens);

// return a vector of tokens
std::vector<Token> tokenize(std::string input);

#endif
