#ifndef CELL_H
#define CELL_H

#include <string>
#include <vector>
#include "Types.h"

using namespace std;

class Agent;  // forward

class Cell {
public:
    string id;
    int row, col;
    int terrain;
    PlayerId controlledBy = PlayerId::NONE;
    bool visible[2] = {false, false};
    vector<Agent*> agents;

    vector<Cell*> neighbors;

    Cell(const string& _id, int _row, int _col, int _terrain);

    void markVisible(PlayerId player);
    bool isVisibleFor(PlayerId player) const;

    bool isEmpty() const { return agents.empty(); }

    string getDisplayText() const;
};

#endif // CELL_H
