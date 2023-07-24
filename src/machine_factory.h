#ifndef MACHINE_FACTORY_H
#define MACHINE_FACTORY_H

#include "state_machine.h"
#include "pattern.h"
#include <string>

class StateMachineFactory {
public:
    StateMachine match_pattern(const Pattern& pattern);
    // creates state machines
    StateMachine match_characters(const std::string& chars);
    StateMachine match_char(const char& c);
    StateMachine success_machine();

private:
    void append_char(StateMachine& m, const char& c);
    void append_range(StateMachine& m, const char& start, const char& end);
    void append_dot(StateMachine& m);
    void append_alphanumeric(StateMachine& m);

    void post_modification(StateMachine& m, const Pattern& pattern);
    // machines need to connect to the second state
    // for example, ab("aab")
    void add_loops_to_second(StateMachine& m, char token);
    void add_all_optional_chars(StateMachine& m, const Pattern& pattern);
    void make_optional(StateMachine& m, int state);
};

#endif
