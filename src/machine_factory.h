#ifndef MACHINE_FACTORY_H
#define MACHINE_FACTORY_H

#include "state_machine.h"
#include <string>

class StateMachineFactory {
public:
    // creates state machines
    StateMachine match_characters(std::string chars);
    StateMachine match_dot();
private:
    // helper functions
    StateMachine match_char(char c);
};

#endif
