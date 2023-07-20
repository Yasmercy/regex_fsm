#include <iostream>
#include <fstream>
#include "machine_factory.h"

// creating a machine that matches abc and converting to json
void write_abc() {
    StateMachineFactory factory;
    auto fsm = factory.match_characters("ab");
    nlohmann::json j = fsm;

    // writing the file
    std::string filename = "fsm/ab.json";
    std::ofstream ofs(filename);
    ofs << std::setw(4) << j << std::endl;
}

int main() {
    write_abc();
}
