#ifndef STATE_MACHINE_H
#define STATE_MACHINE_H

#include "../inc/json.hpp"
#include <map>
#include <set>
#include <utility>
#include <vector>

class StateMachine {
    using State = int;
    using Token = char;
    using AdjacencyList = std::map<std::pair<State, Token>, State>;
public:
    // constructor
    StateMachine();
    StateMachine(int num_states);
    // mutators API
    void add_transition(State start, Token token, State end);
    void remove_transition(State start, Token token);
    void increase_num_states(int new_num_states); // must be >= current
    void increase_num_states_by(int increase);
    // methods API
    int get_num_states();
    State get_start();
    State get_failure();
    State get_success();
    State get_output(const std::vector<Token>& tokens);
    State get_output(const State& start, const std::vector<Token>& tokens);
    State next_state(const State& start, const Token& token);
    Token get_else_action();
    AdjacencyList get_transition();
    bool valid_start_state(State state);
    bool valid_end_state(State state);

    // for json conversions
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(
        StateMachine, 
        transition,
        start_state, 
        success_state, 
        fail_state, 
        num_states
    )
private:
    // maps a tuple of state and action to a new state
    AdjacencyList transition;
    
    // important states
    State start_state; // starting state
    State success_state; // the state that returns true (for a matching)
    State fail_state; // the state that returns false (for a matching)
    // counters
    int num_states;
};

#endif
