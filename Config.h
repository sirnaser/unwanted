#ifndef CONFIG_H
#define CONFIG_H

#include "Types.h"
#include <string>

using namespace std;

class Config {
public:
    static WinCondition load(const string& filename = "resources/config.txt");
    static WinCondition defaultWin() { return WinCondition(); }
};

#endif // CONFIG_H
