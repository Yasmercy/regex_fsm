#include "state_machine.h"

using State = int;
using Action = int;
using Token = char;

StateMachine::StateMachine() : start_state(0), success_state(-1), fail_state(-2), num_states(0) {}

StateMachine::StateMachine(unsigned int num_states) : start_state(0), success_state(-1), fail_state(-2), num_states(num_states) {}

void StateMachine::add_transition(State start, Token token, State end) {
    transition[{start, token}] = end;
}

void StateMachine::add_start_transition(State start, Token token) {
    add_transition(start, token, start_state);
}

void StateMachine::add_success(State start, Token token) {
    add_transition(start, token, success_state);
}

void StateMachine::add_failure(State start, Token token) {
    add_transition(start, token, fail_state);
}

void StateMachine::add_match_all_transition(State start, State end) {
    match_all_transitions[start] = end;
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

State StateMachine::next_state(const State& start, const Token& token) {
    if (transition.contains({start, token})) {
        return transition[{start, token}];
    } else if (to_start.contains({start, token})) {
        return start_state;
    }
    return match_all_transitions[start]; // can error
}

State StateMachine::get_output(const State& start, const std::vector<Token>& tokens) {
    State current = start;
    for (Token token : tokens) {
        current = next_state(current, token);
    }
    return current;
}

State StateMachine::get_output(const std::vector<Token>& tokens) {
    return get_output(start_state, tokens);
}

int StateMachine::get_num_states() {
    return num_states;
}

void StateMachine::append(const StateMachine& other) {
    // this just maps all the success states of this to the start of other
    ++num_states;
    // connect all success endpoints to the other's start
    for (const auto& ele : transition) {
        auto [start, action] = ele.first;
        auto end = ele.second;
        if (end == success_state) {
            transition[{start, action}] = num_states;
        }
    }
    for (const auto& ele : match_all_transitions) {
        auto start = ele.first;
        auto end = ele.second;
        if (end == success_state) {
            match_all_transitions[start] = num_states;
        }
    }
    
    // remap all nodes in other to new nodes in this
    // exclude the special nodes in other (success and fail)
    for (const auto& ele : other.transition) {
        auto [start, action] = ele.first;
        auto end = ele.second;
        start += num_states;
        end += num_states * (end != success_state && end != fail_state);
        transition[{start, action}] = end;
    }
    for (const auto& ele : other.match_all_transitions) {
        auto start = ele.first;
        auto end = ele.second;
        start += num_states;
        end += num_states * (end != success_state && end != fail_state);
        match_all_transitions[start] = end;
    }

    // add the other start states to this
    for (const auto& [start, token] : other.to_start) {
        to_start.insert({start + num_states, token});
    }
}

bool StateMachine::valid_state(State state) {
    // TODO fix (-2 <= state < num_states)
    int N = num_states;
    return (-2 <= state) && (state <= N);
}
