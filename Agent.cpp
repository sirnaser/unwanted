#include "Agent.h"

Agent::Agent(AgentType t, PlayerId o, int h) : type(t), owner(o) {
    if (h == 0) {
        if (t == AgentType::SCOUT) hp = 5;
        else if (t == AgentType::SNIPER) hp = 4;
        else hp = 3;
    } else {
        hp = h;
    }
}

bool Agent::canMoveTo(Cell* target) const {
    // Basic adjacency check (full logic in GameEngine)
    return true;
}

int Agent::getBaseAttackValue() const {
    return (type == AgentType::SCOUT) ? 5 : (type == AgentType::SNIPER) ? 4 : 3;
}

void Agent::takeDamage(int dmg) {
    hp = max(0, hp - dmg);
    if (hp <= 0 && position) {
        // Remove from cell (handled in Engine)
    }
}

string Agent::toString() const {
    if (type == AgentType::SCOUT) return "S";
    if (type == AgentType::SNIPER) return "N";
    return "G";
}
