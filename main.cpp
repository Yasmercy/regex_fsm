#include "nfa.h"
#include "json.hpp"
#include <iostream>
#include <fstream>

int main() {
    // NFA machine;
    // 
    // NFA a {'a'};
    // NFA b {'b'};
    // NFA c {'c'};
    // b.kleene_star();

    // machine.concat(std::move(a));
    // machine.concat(std::move(b));
    // machine.concat(std::move(c));

    NFA machine("(a*|b*)|(cd*)");

    std::ofstream o("dump.json");
    nlohmann::json j = machine;
    o << std::setw(4) << j << std::endl;
}
