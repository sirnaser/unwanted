#include "Config.h"
#include "Utils.h"
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// Local trim function
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}

WinCondition Config::load(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Config file not found → using default (control=7)" << endl;
        return defaultWin();
    }

    WinCondition wc;
    string line;

    while (getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;

        size_t eq = line.find('=');
        if (eq == string::npos) continue;

        string key = trim(line.substr(0, eq));
        string val = trim(line.substr(eq + 1));

        if (key == "win_type") wc.type = val;
        else if (key == "win_value") {
            try { wc.value = stoi(val); } catch (...) { wc.value = 7; }
        }
    }

    file.close();
    return wc;
}
