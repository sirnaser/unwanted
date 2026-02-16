#ifndef PLAYER_H
#define PLAYER_H

#include "Deck.h"
#include "Agent.h"
#include "Types.h"
#include <set>
#include <string>

using namespace std;

class Player {
public:
    string name;
    PlayerId id;
    Deck deck;
    vector<Agent*> agents;
    set<Cell*> controlledCells;

    Player(const string& n, PlayerId i);

    void initAgents(const string& agentsFile, Board* board);  // forward in .cpp

    bool hasWon(const WinCondition& wc) const;

    string getStatus() const;
};

#endif // PLAYER_H
