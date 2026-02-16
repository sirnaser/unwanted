#include "Player.h"
#include "Board.h"
#include "Utils.h" // for trim
#include <fstream>
#include <iostream>
#include <sstream>

Player::Player(const string& n, PlayerId i) : name(n), id(i) {}

Player::~Player() {
    for (Agent* a : agents) delete a;
}

void Player::initAgents(const string& agentsFile, Board* board) {
    if (!board) {
        cout << "Board pointer is null" << endl;
        return;
    }

    ifstream file(agentsFile);
    if (!file.is_open()) {
        cout << "Agents file not found: " << agentsFile << " - using fallback" << endl;
        // fallback: 3 agents in top-left cells
        if (Cell* c = board->getCell(0, 0)) {
            Agent* a = new Agent(AgentType::SCOUT, id);
            c->agents.push_back(a);
            a->position = c;
            agents.push_back(a);
        }
        // add more if needed
        return;
    }

    string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        size_t colon = line.find(':');
        if (colon == string::npos) continue;

        string cellId = trim(line.substr(0, colon));
        string rest = trim(line.substr(colon + 1));

        size_t comma = rest.find(',');
        if (comma == string::npos) continue;

        string ownerStr = trim(rest.substr(0, comma));
        string typeStr = trim(rest.substr(comma + 1));

        char ownerChar = toupper(ownerStr.empty() ? ' ' : ownerStr[0]);
        if ((ownerChar == 'A' && id != PlayerId::PLAYER1) ||
            (ownerChar == 'B' && id != PlayerId::PLAYER2)) continue;

        Cell* target = board->getCell(cellId);
        if (!target) {
            cout << "Cell not found: " << cellId << endl;
            continue;
        }

        AgentType atype = AgentType::SCOUT;
        if (typeStr == "Sniper") atype = AgentType::SNIPER;
        else if (typeStr == "Seargeant" || typeStr == "Sergeant") atype = AgentType::SERGEANT;

        Agent* agent = new Agent(atype, id);
        target->agents.push_back(agent);
        agent->position = target;
        agents.push_back(agent);
    }

    file.close();
}

bool Player::hasWon(const WinCondition& wc) const {
    if (wc.type == "eliminate") return agents.empty();
    if (wc.type == "control") return controlledCells.size() >= static_cast<size_t>(wc.value);
    return false;
}

string Player::getStatus() const {
    return name + " (" + to_string(getLivingAgentsCount()) +
           " agents, " + to_string(controlledCells.size()) + " controlled)";
}

int Player::getLivingAgentsCount() const {
    int count = 0;
    for (const Agent* a : agents) if (a->hp > 0) count++;
    return count;
}
