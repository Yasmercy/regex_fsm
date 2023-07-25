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
    StateMachine match_atom(const Atom& atom, const Modifier& mod);

private:
    void append_atom(StateMachine& m, const Atom& atom);
    void append_repeated_atom(StateMachine& m, const Atom& atom);
    void post_modification(StateMachine& m, const Pattern& pattern);
    // machines need to connect to the second state
    // for example, ab("aab")
    void add_backloops(StateMachine& m, const Atom& token, int state);
    void add_all_backloops(StateMachine& m, const Pattern& pattern);
    void add_all_optional_chars(StateMachine& m, const Pattern& pattern);
    void make_optional(StateMachine& m, const std::vector<Atom>& atoms, int start, int end);
    bool is_optional(Modifier mod);
};

#endif
