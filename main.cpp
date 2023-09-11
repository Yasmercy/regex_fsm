#include "nfa.h"
#include "json.hpp"
#include <iostream>
#include <fstream>

int main() {
    // NFA machine("(a|b)|(cd*)");
    NFA machine("a*");
    machine.prune_epsilon();

    std::ofstream o("dump.json");
    nlohmann::json j = machine;
    o << std::setw(4) << j << std::endl;

    // testing accepting inputs
    State test1 = machine.simulate("a");
    State test2 = machine.simulate("b");
    State test3 = machine.simulate("c");
    State test4 = machine.simulate("cdd");

    // testing non-accepting inputs
    State test5 = machine.simulate("");
    State test6 = machine.simulate("d");
    State test7 = machine.simulate("e");

    std::cout << machine.is_terminal(test1) << "\n";
    std::cout << machine.is_terminal(test2) << "\n";
    std::cout << machine.is_terminal(test3) << "\n";
    std::cout << machine.is_terminal(test4) << "\n";
    std::cout << machine.is_terminal(test5) << "\n";
    std::cout << machine.is_terminal(test6) << "\n";
    std::cout << machine.is_terminal(test7) << "\n";
}
