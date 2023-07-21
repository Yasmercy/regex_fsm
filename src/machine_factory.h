#ifndef MACHINE_FACTORY_H
#define MACHINE_FACTORY_H

#include "state_machine.h"
#include "pattern.h"
#include <string>

class StateMachineFactory {
public:
    // creates state machines
    StateMachine match_characters(const std::string& chars);
    StateMachine match_char(const char& c);
    StateMachine success_machine();

private:
    void append_char(StateMachine& m, const char& c);
    void post_modification(StateMachine& m, const Pattern& pattern);
    
    // machines need to connect to the second state
    // for example, ab("aab") 
    void add_loops_to_second(StateMachine& m, char token); 
};

#endif
