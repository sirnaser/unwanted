#ifndef BOARD_H
#define BOARD_H

#include "Cell.h"
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

    void loadFromFile(const string& filename);

    void precomputeNeighbors();

    Cell* getCell(int row, int col) const;
    Cell* getCell(const string& id) const;
    vector<Cell*> getAllCells() const;

    vector<Cell*> getNeighbors(Cell* cell) const;

    ~Board();
};

#endif // BOARD_H
