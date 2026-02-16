#ifndef PLAYER_H
#define PLAYER_H

#include "Deck.h"
#include "Agent.h"
#include "Types.h"
#include <set>
#include <string>
#include <vector>

using namespace std;

class Player {
public:
    string name;
    PlayerId id;
    Deck deck;
    vector<Agent*> agents;           // Owned units on the board
    set<Cell*> controlledCells;      // Cells under this player's control

    Player(const string& playerName, PlayerId playerId);
    void initAgents(const string& agentsFile, Board* board);
    bool hasWon(const WinCondition& winCond) const;

    string getStatus() const;
    int getLivingAgentsCount() const;
    ~Player();
};

#endif // PLAYER_H
