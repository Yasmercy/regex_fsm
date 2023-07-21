#include "machine_factory.h"

using Factory = StateMachineFactory;
char END_CHAR = '\0'; // check this

StateMachine Factory::match_char(char c) {
    StateMachine m;
    m.add_success(m.get_start(), c);
    m.add_failure(m.get_start(), END_CHAR);
    // m.add_match_all_transition(m.get_start(), m.get_start());
    m.add_start_transition(m.get_start(), 'x');
    return m;
}

StateMachine Factory::match_characters(const std::string& chars) {
    if (chars.empty()) {
        StateMachine m;
        return m;
    }

    StateMachine m = match_char(chars[0]);
    bool first_flag = true;
    for (char c : chars) {
        if (first_flag) {
            first_flag = false;
            continue;
        }
        m.append(match_char(c));
    }

    return m;
}
