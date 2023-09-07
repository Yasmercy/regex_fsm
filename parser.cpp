#include "parser.h"

std::vector<Token> tokenize(std::string input) {
    std::vector<Token> out;

    for (char c : input) {
        switch (c) {
            case '(':
                out.push_back(LeftParen{});
                break;
            case ')':
                out.push_back(RightParen{});
                break;
            case '|':
                out.push_back(Alternate{});
                break;
            case '*':
                out.push_back(KleeneStar{});
                break;
            case ' ':
            case '\n':
            case '\t':
                break;
            default:
                out.push_back(c);
        }
    }

    return out;
}

template <class ...Fs>
struct overload : Fs... {
  overload(Fs const&... fs) : Fs{fs}...
  {}

  using Fs::operator()...;
};

bool check_parens_match(const std::vector<Token>& tokens) {
    int paren_counter;
    bool err_flag = false;
    for (auto token : tokens) {
        if (err_flag) {
            break;
        }

        std::visit(
            overload(
                [&](LeftParen) {
                    ++paren_counter;
                },
                [&](RightParen) {
                    --paren_counter;
                    err_flag = paren_counter < 0;
                },
                [](auto&&) {}
            ),
            token
        );
    }

    return paren_counter == 0;
}

bool valid(const std::vector<Token>& tokens) {
    return check_parens_match(tokens);
}

