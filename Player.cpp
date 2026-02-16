#include "Player.h"
#include "Board.h"
#include <fstream>
#include <iostream>
#include <sstream>

Player::Player(const string& playerName, PlayerId playerId)
    : name(playerName), id(playerId) {
    // Deck and agents will be initialized later
}

Player::~Player() {
    // Clean up owned agents
    for (Agent* agent : agents) {
        delete agent;
    }
    agents.clear();
    controlledCells.clear();
}

void Player::initAgents(const string& agentsFile, Board* board) {
    if (!board) {
        cout << "Error: Board pointer is null in initAgents" << endl;
        return;
    }

    ifstream file(agentsFile);
    if (!file.is_open()) {
        cout << "Warning: Could not open agents file: " << agentsFile << endl;
        cout << "Using fallback default agents" << endl;

        // Fallback: place 3 agents in fixed positions (example)
        // Adjust coordinates based on your board size
        Cell* c1 = board->getCell(0, 0);
        Cell* c2 = board->getCell(0, 1);
        Cell* c3 = board->getCell(0, 2);

        if (c1) {
            Agent* a1 = new Agent(AgentType::SCOUT, id);
            c1->agents.push_back(a1);
            a1->position = c1;
            agents.push_back(a1);
        }
        if (c2) {
            Agent* a2 = new Agent(AgentType::SNIPER, id);
            c2->agents.push_back(a2);
            a2->position = c2;
            agents.push_back(a2);
        }
        if (c3) {
            Agent* a3 = new Agent(AgentType::SERGEANT, id);
            c3->agents.push_back(a3);
            a3->position = c3;
            agents.push_back(a3);
        }

        return;
    }

    string line;
    while (getline(file, line)) {
        line = trim(line);
        if (line.empty()) continue;

        // Expected format example: A04:A,Sniper
        size_t colon = line.find(':');
        if (colon == string::npos) continue;

        string cellId = trim(line.substr(0, colon));
        string rest   = trim(line.substr(colon + 1));

        size_t comma = rest.find(',');
        if (comma == string::npos) continue;

        string ownerStr = trim(rest.substr(0, comma));
        string typeStr  = trim(rest.substr(comma + 1));

        // Only process agents for this player
        char ownerChar = toupper(ownerStr[0]);
        if ((ownerChar == 'A' && id != PlayerId::PLAYER1) ||
            (ownerChar == 'B' && id != PlayerId::PLAYER2)) {
            continue;
        }

        Cell* targetCell = board->getCell(cellId);
        if (!targetCell) {
            cout << "Warning: Cell not found for agent: " << cellId << endl;
            continue;
        }

        AgentType atype = AgentType::SCOUT;
        if (typeStr == "Sniper")    atype = AgentType::SNIPER;
        else if (typeStr == "Seargeant" || typeStr == "Sergeant") atype = AgentType::SERGEANT;

        Agent* newAgent = new Agent(atype, id);
        targetCell->agents.push_back(newAgent);
        newAgent->position = targetCell;
        agents.push_back(newAgent);
    }

    file.close();
    cout << "Player " << name << " initialized " << agents.size() << " agents." << endl;
}

bool Player::hasWon(const WinCondition& winCond) const {
    if (winCond.type == "eliminate") {
        return agents.empty();
    }
    else if (winCond.type == "control") {
        return controlledCells.size() >= static_cast<size_t>(winCond.value);
    }
    return false;
}

string Player::getStatus() const {
    return name + " (" + to_string(getLivingAgentsCount()) +
           " agents, " + to_string(controlledCells.size()) + " controlled)";
}

int Player::getLivingAgentsCount() const {
    int count = 0;
    for (const Agent* a : agents) {
        if (a->hp > 0) count++;
    }
    return count;
}
