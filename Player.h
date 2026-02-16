#ifndef PLAYER_H
#define PLAYER_H

#include "Deck.h"
#include "Agent.h"
#include "Types.h"
#include <set>
#include <string>
#include <vector>

using namespace std;

class Board; // forward declaration

class Player {
public:
    string name;
    PlayerId id;
    Deck deck;
    vector<Agent*> agents;
    set<Cell*> controlledCells;

    Player(const string& n, PlayerId i);

    void initAgents(const string& agentsFile, Board* board);

    bool hasWon(const WinCondition& wc) const;
    string getStatus() const;
    int getLivingAgentsCount() const;

    ~Player();
};

#endif // PLAYER_H
