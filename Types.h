#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <vector>

using namespace std;

enum class PlayerId { NONE, PLAYER1, PLAYER2 };

enum class AgentType { SCOUT, SNIPER, SERGEANT };

enum class ActionType {
    MOVE,
    ATTACK,
    SCOUT,
    CONTROL,
    RELEASE
};

struct WinCondition {
    string type;  // "control" or "eliminate"
    int value;

    WinCondition() : type("control"), value(7) {}
    WinCondition(const string& t, int v) : type(t), value(v) {}
};

#endif // TYPES_H
