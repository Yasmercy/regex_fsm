#ifndef STATES_H
#define STATES_H

#include <compare>
#include "json.hpp"

struct State {
    static int num_states;

    State() : id{++num_states} {}
    int id;

    auto operator<=>(const State& other) const = default;   
    
    NLOHMANN_DEFINE_TYPE_INTRUSIVE(State, id);
};

#endif
