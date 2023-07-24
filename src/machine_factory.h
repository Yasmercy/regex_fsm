#ifndef MACHINE_FACTORY_H
#define MACHINE_FACTORY_H

#include "state_machine.h"
#include "pattern.h"
#include <string>

class StateMachineFactory {
public:
    StateMachine match_pattern(const Pattern& pattern);
    // creates state machines
    StateMachine success_machine();
    StateMachine match_atom(const Atom& atom);

private:
    void append_atom(StateMachine& m, const Atom& atom);
    void post_modification(StateMachine& m, const Pattern& pattern);
    // machines need to connect to the second state
    // for example, ab("aab")
    void add_loops_to_second(StateMachine& m, char token);
    void add_all_optional_chars(StateMachine& m, const Pattern& pattern);
    void make_optional(StateMachine& m, int state);
};

#endif
