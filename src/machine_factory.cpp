#include "machine_factory.h"

using Factory = StateMachineFactory;
char END_CHAR = '\0'; // check this

StateMachine Factory::match_char(char c) {
    StateMachine m;
    m.add_success(m.get_start(), c);
    m.add_failure(m.get_start(), END_CHAR);
    m.add_match_all_transition(m.get_start(), m.get_start());
    return m;
}

StateMachine Factory::match_characters(std::string chars) {
    StateMachine m;
    for (char c : chars) {
        m.append(match_char(c));
    }

    return m;
}
