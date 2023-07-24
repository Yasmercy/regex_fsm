#include <iostream>
#include <fstream>
#include "regex.h"

// creating a machine that matches abc and converting to json
void write_abc() {
    StateMachineFactory factory;
    std::string chars = "abc?";
    std::string file = "abc_";
    Pattern p {chars};
    auto fsm = factory.match_pattern(p);
    nlohmann::json j = fsm;

    // writing the file
    std::string filename = "fsm/" + file + ".json";
    std::ofstream ofs(filename);
    ofs << std::setw(4) << j << std::endl;
}

void playground() {
    RegEx regex{"abc"};
    auto j = regex.get_machine_json();
    std::cout << j["transition"] << std::endl;
    std::cout << regex.match("aaa").matched;
}

int main() {
    // write_abc();
    playground();
}
