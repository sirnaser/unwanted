#ifndef BOARD_H
#define BOARD_H

#include "Cell.h"
#include "Agent.h"
#include "Types.h"
#include <vector>
#include <map>
#include <string>

using namespace std;

class Board {
public:
    int rows = 0;
    int cols = 0;
    vector<vector<Cell*>> grid;
    map<string, Cell*> byId;

    Board();
    ~Board();

    void load(const string& boardFile, const string& initFile);
    void precomputeNeighbors();

    Cell* getCell(int r, int c) const;
    Cell* getCell(const string& id) const;
    vector<Cell*> getAllCells() const;
    vector<Cell*> getNeighbors(Cell* cell) const;

    void print() const;
};

#endif // BOARD_H
