#ifndef AGENT_H
#define AGENT_H

#include "Types.h"
#include "Cell.h"

using namespace std;

class Agent {
public:
    AgentType type;
    int hp;
    PlayerId owner;
    Cell* position = nullptr;

    Agent(AgentType t, PlayerId o, int h = 0);

    bool canMoveTo(Cell* target) const;

    int getBaseAttackValue() const;

    void takeDamage(int dmg);

    string toString() const;
};

#endif // AGENT_H
