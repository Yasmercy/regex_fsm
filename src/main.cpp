#include <iostream>
#include <fstream>
#include "machine_factory.h"
#include "regex.h"

// creating a machine that matches abc and converting to json
void write_abc() {
    StateMachineFactory factory;
    std::string chars = "abc";
    auto fsm = factory.match_characters(chars);
    nlohmann::json j = fsm;

    // writing the file
    std::string filename = "fsm/" + chars + ".json";
    std::ofstream ofs(filename);
    ofs << std::setw(4) << j << std::endl;
}

void playground() {
    RegEx regex("ab");
    std::cout << "ab-aab: " << regex.match("aab").matched << "\n";
    std::cout << "ab-axb: " << regex.match("axb").matched << "\n";
    
    RegEx regex1("abc");
    std::cout << "abc-acbc: " << regex1.match("acbc").matched << "\n";
}

int main() {
    write_abc();
    // playground();
}
