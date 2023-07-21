#include "state_machine.h"
#include <stdexcept>
#include <iostream>

using State = int;
using Token = char;
using AdjacencyList = std::map<std::pair<State, Token>, State>; 
constexpr Token ELSE = '\1';

StateMachine::StateMachine() : start_state(0), success_state(-1), fail_state(-2), num_states(0) {}

StateMachine::StateMachine(int num_states) : start_state(0), success_state(-1), fail_state(-2), num_states(num_states) {}

void StateMachine::add_transition(State start, Token token, State end) {
    if (!valid_start_state(start) || !valid_end_state(end)) {
        throw std::runtime_error("invalid state for connection");
    }
    transition[{start, token}] = end;
}

void StateMachine::remove_transition(State start, Token token) {
    transition.erase({start, token});
}

void StateMachine::increase_num_states(int new_num_states) {
    if (new_num_states < num_states) {
        throw std::runtime_error("cannot decrease # of states");
    }
    num_states = new_num_states;
}

void StateMachine::increase_num_states_by(int increase) {
    increase_num_states(num_states + increase);
}

int StateMachine::get_num_states() {
    return num_states;
}

State StateMachine::get_start() {
    return start_state;
}

State StateMachine::get_failure() {
    return fail_state;
}

State StateMachine::get_success() {
    return success_state;
}

Token StateMachine::get_else_action() {
    return ELSE;
}

AdjacencyList StateMachine::get_transition() {
    return transition;
}

State StateMachine::next_state(const State& start, const Token& token) {
    if (transition.contains({start, token})) {
        return transition[{start, token}];
    }
    return transition.at({start, ELSE}); // can error
}

State StateMachine::get_output(const State& start, const std::vector<Token>& tokens) {
    State current = start;
    for (Token token : tokens) {
        std::cout << current << "->";
        current = next_state(current, token);
    }
    std::cout << current << "\n";
    return current;
}

State StateMachine::get_output(const std::vector<Token>& tokens) {
    return get_output(start_state, tokens);
}

bool StateMachine::valid_start_state(State state) {
    // exclude fail and success
    return (start_state <= state) && (state <= num_states);
}

bool StateMachine::valid_end_state(State state) {
    return (fail_state <= state) && (state <= num_states);
}
