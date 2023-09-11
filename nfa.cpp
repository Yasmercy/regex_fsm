#include <algorithm>
#include <iostream>
#include "nfa.h"
#include "parser.h"
#include "token.h"

NFA::NFA() {
    start = State();
    State end = State();
    terminals = {end};
    insert_transition(start, end, Epsilon);
}

NFA::NFA(Symbol s) {
    start = State();
    State end = State();
    terminals = {end};
    insert_transition(start, end, s);
}

void NFA::kleene_star() {
    State new_start;
    State new_end;

    // connect new_start to start
    insert_transition(new_start, start, Epsilon);

    // connect all terminals
    for (State terminal : terminals) {
        insert_transition(terminal, start, Epsilon);
        insert_transition(terminal, new_end, Epsilon);
    }

    // connect new_start and new_end
    insert_transition(new_start, new_end, Epsilon);

    // update start and end
    start = new_start;
    terminals = { new_end };
}

void NFA::concat(NFA&& other) {
    // connect this end to start of other
    for (State terminal : terminals) {
        insert_transition(terminal, other.start, Epsilon);
    }

    // copy over other's state and transition function
    transition.merge(other.transition);

    // set the new end
    terminals = other.terminals;
}

void NFA::alternation(NFA&& other) {
    State new_start;
    State new_end;

    // connect new_start to start
    insert_transition(new_start, start, Epsilon);
    insert_transition(new_start, other.start, Epsilon);

    // connect terminals to new_end
    for (State terminal : terminals) {
        insert_transition(terminal, new_end, Epsilon);
    }
    for (State terminal : other.terminals) {
        insert_transition(terminal, new_end, Epsilon);
    }
    
    // copy over other's state and transition function
    transition.merge(other.transition);

    // update start and end
    start = new_start;
    terminals = { new_end };
}

bool NFA::is_terminal(const State& state) {
    return std::find(terminals.begin(), terminals.end(), state) != terminals.end();
}

State NFA::backtrack(const std::vector<Symbol>& input, State cur, std::size_t input_index) {
    // base case 1: reached end state
    if (is_terminal(cur)) {
        return cur;
    }

    // feed forward (non-consuming)
    for (const auto& n : transition[{cur, Epsilon}]) {
        // don't consume an input
        State f = backtrack(input, n, input_index);
        if (is_terminal(f)) {
            return f;
        }
    }

    // base case 2: reached end of inputs
    if (input_index == input.size()) {
        return cur;
    }

    // feed forward,(consuming) 
    for (const auto& n : transition[{cur, input[input_index]}]) {
        // consume an input
        State f = backtrack(input, n, input_index + 1);
        if (is_terminal(f)) {
            return f;
        }
    }

    // no way to reach a terminal state
    return cur;
}

State NFA::simulate(const std::string& input) {
    std::vector<Symbol> chars(input.begin(), input.end());
    return backtrack(chars, start, 0);
}

void NFA::insert_transition(State start, State end, Symbol action) {
    // transition.insert(std::make_pair(std::make_pair(start, action), end));
    transition[{start, action}].insert(end);
}

template <class ...Fs>
struct overload : Fs... {
  overload(Fs const&... fs) : Fs{fs}...
  {}

  using Fs::operator()...;
};

Token next_operator(std::vector<Token>& tokens) {
    // takes the next token if its an operator
    // otherwise returns concat
    Token out;
    std::visit(
        overload(
            [&](Alternate) {
                tokens.pop_back();
                out = Alternate {};
            },
            [&](auto&&) {
                out = Concatenate {};
            }
        ),
        tokens.back()
    );
    return out;
}

std::vector<Token> next_operand(std::vector<Token>& tokens) {
    // takes all tokens needed for next operand
    // includes everything in a paren
    // includes all kleene_star modifiers
    // if wrapped in a parens, strip that

    std::vector<Token> out;

    // check if paren
    if (std::holds_alternative<LeftParen>(tokens.back())) {
        tokens.pop_back();
        int paren_counter = 1;
        while (paren_counter != 0) {
            // take until reach paren_counter = 0
            if (std::holds_alternative<LeftParen>(tokens.back())) {
                ++paren_counter;
            } else if (std::holds_alternative<RightParen>(tokens.back())) {
                --paren_counter;
            }
            out.push_back(tokens.back());
            tokens.pop_back();
        }
        out.pop_back();
        std::reverse(out.begin(), out.end());
        return out;
    }

    // return next char and takewhile stars
    out.push_back(tokens.back());
    tokens.pop_back();
    while (!tokens.empty() && std::holds_alternative<KleeneStar>(tokens.back())) {
        out.push_back(tokens.back());
        tokens.pop_back();
    }
    std::reverse(out.begin(), out.end());
    return out;
}

bool trivial(const std::vector<Token>& tokens) {
    // returns whether there are < 2 chars and no operations besides stars
    int char_counter = 0;
    for (auto token : tokens) {
        if (std::holds_alternative<char>(token)) {
            ++char_counter;
        } else if (!std::holds_alternative<KleeneStar>(token)) {
            return false;
        }
    }
    return char_counter < 2;
}

NFA build_trivial(const std::vector<Token>& tokens) {
    // precondition: tokens correspond to a trivial NFA
    // an ordered tokens (not reversed)
    NFA out;
    for (auto token : tokens) {
        if (const char* val = std::get_if<char>(&token)) {
            out = NFA{*val};
        } else {
            out.kleene_star();
        }
    }
    return out;
}

void extend(const Token& op, NFA& nfa, NFA&& other) {
    std::visit(
        overload(
            [&](Alternate) {
                nfa.alternation(std::move(other));
            },
            [&](Concatenate) {
                nfa.concat(std::move(other));
            },
            [&](auto&&) {
                std::cout << "crash and burn\n";
            }
        ),
       op 
    );
}

NFA build_nfa(std::vector<Token>& tokens) {
    // builds an NFA from a set of tokens (reversed)
    
    // base case: only chars and stars
    if (trivial(tokens)) {
        std::reverse(tokens.begin(), tokens.end());
        return build_trivial(tokens);
    }

    // start with a default NFA
    NFA out;

    // iterate over next tokens
    // recurse as needed
    while (!tokens.empty()) {
        // get the next operator
        Token op = next_operator(tokens);
        // get the next operand
        std::vector<Token> next = next_operand(tokens);
        NFA operand = build_nfa(next);
        // do the operation
        extend(op, out, std::move(operand));
    }
    return out;
}

NFA::NFA(std::string pattern) {
    std::vector<Token> tokens = tokenize(pattern);
    std::reverse(tokens.begin(), tokens.end());
    *this = build_nfa(tokens);
}

void NFA::prune_epsilon() {
    const auto terminals_copy = terminals;
    for (const auto& terminal : terminals_copy) {
        prune_epsilon_helper(terminal);
    }
    prune_unreachable();
}

void NFA::prune_epsilon_helper(const State& cur) {
    // prev --e-- cur
    // add al of cur's connections to prev
    // set prev to final if cur is final
    // delete the epsilon transition
    // continue recursing down prev

    const std::set<State> prevs = parent_epsilon(cur);
    const std::set<Symbol> actions = get_all_actions(cur);
    std::set<State> next = parent(cur);
    next.insert(prevs.begin(), prevs.end());

    for (const auto& prev : prevs) {
        // add connections
        for (const auto& action : actions) {
            auto endpoints = transition[{cur, action}];
            transition[{prev, action}].merge(endpoints);
        }

        // set terminal
        if (is_terminal(cur)) {
            terminals.push_back(prev);
        }
        // delete epsilon transition
        transition[{prev, Epsilon}].erase(cur);
    }

    // recurse
    for (const auto& n : next) {
        prune_epsilon_helper(n);
    }
}

std::set<State> NFA::parent_epsilon(const State& cur) {
    std::set<State> out;
    for (const auto& [key, value] : transition) {
        if (const auto search = value.find(cur); search != value.end() && key.second == Epsilon) {
            out.insert(key.first);
        }
    }
    return out;
}

std::set<State> NFA::parent(const State& cur) {
    std::set<State> out;
    for (const auto& [key, value] : transition) {
        if (const auto search = value.find(cur); search != value.end()) {
            out.insert(key.first);
        }
    }
    return out;
}

std::set<Symbol> NFA::get_all_actions(const State& cur) {
    std::set<Symbol> out;
    for (const auto& [key, _] : transition) {
        if (key.first == cur) {
            out.insert(key.second);
        }
    }
    return out;
}

void NFA::prune_unreachable() {
    // collect all the reachable states with a dfs from start
    std::set<State> reachable;
    std::set<State> stack = {start};

    while (!stack.empty()) {
        State cur = *stack.begin();
        stack.erase(cur);

        // continue if it was already reached (skip cycles)
        if (auto search = reachable.find(cur); search != reachable.end()) {
            continue;
        }
        reachable.insert(cur);

        // add all to stack
        for (auto action : get_all_actions(cur)) {
            std::set<State> endpoint = transition[{cur, action}];
            stack.insert(endpoint.begin(), endpoint.end());
        }
    }

    const auto transition_copy = transition;
    for (const auto& [key, value] : transition_copy) {
        if (auto search = reachable.find(key.first); search == reachable.end()) {
            transition.erase(key);
        }
    }
}
