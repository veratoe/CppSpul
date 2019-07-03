#include <iostream>
#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace std;

string readFile(string fileName) {
    ifstream file(fileName);
    string content( (istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

    return content;

}

int main() {

    std::cout << "File lezen..." << std::endl;

    string diede_json = readFile("proef.json");
    std::cout << "Done" << std::endl;
    std::cout << "JSON parsen.." << std::endl;

    auto diede = json::parse(diede_json);
    
    std::cout << "Done!" << std::endl;

    std::cout << "De waarde van Diede is :" << diede["diede"] << endl;

    cout << "We printen getallen" << endl;

    for (int i = 0; i < diede["getallen"].size(); i++) {
        cout << diede["getallen"][i] << endl;
    }

    cout << "Done" << endl;
    return 0;

}