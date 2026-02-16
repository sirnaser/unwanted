#include "Board.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>

Board::Board() = default;

Board::~Board() {
    for (auto& row : grid) {
        for (Cell* cell : row) {
            delete cell;
        }
    }
    grid.clear();
    byId.clear();
}

string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t\r\n|");
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(" \t\r\n|");
    return s.substr(start, end - start + 1);
}

void Board::load(const string& boardFile, const string& initFile) {
    ifstream bf(boardFile);
    if (!bf.is_open()) {
        cerr << "Failed to open board file: " << boardFile << endl;
        return;
    }

    vector<string> lines;
    string line;
    while (getline(bf, line)) {
        line = trim(line);
        if (!line.empty()) lines.push_back(line);
    }
    bf.close();

    if (lines.empty()) {
        cerr << "Board file is empty" << endl;
        return;
    }

    rows = lines.size();
    cols = 0;
    for (const auto& ln : lines) {
        stringstream ss(ln);
        string tk;
        int cnt = 0;
        while (ss >> tk) cnt++;
        cols = max(cols, cnt);
    }

    grid.assign(rows, vector<Cell*>(cols, nullptr));

    int r = 0;
    for (const string& rowStr : lines) {
        stringstream ss(rowStr);
        string token;
        int c = 0;

        while (ss >> token) {
            token = trim(token);
            if (token.empty()) continue;

            size_t colon = token.find(':');
            if (colon == string::npos) continue;

            string cid = token.substr(0, colon);
            string valStr = token.substr(colon + 1);

            int terrain = 0;
            try {
                terrain = stoi(valStr);
                terrain = max(0, min(2, terrain));
            } catch (...) {}

            Cell* cell = new Cell(cid, r, c, terrain);
            grid[r][c] = cell;
            byId[cid] = cell;

            c++;
        }
        r++;
    }

    cout << "Board structure loaded: " << rows << " rows × " << cols << " cols" << endl;

    ifstream initf(initFile);
    if (!initf.is_open()) {
        cout << "No initial state file found: " << initFile << endl;
        precomputeNeighbors();
        return;
    }

    while (getline(initf, line)) {
        line = trim(line);
        if (line.empty()) continue;

        size_t comma = line.find(',');
        if (comma == string::npos) continue;

        string left  = trim(line.substr(0, comma));
        string right = trim(line.substr(comma + 1));

        size_t colon = left.find(':');
        if (colon == string::npos) continue;

        string cellId = trim(left.substr(0, colon));
        string ownerPart = trim(left.substr(colon + 1));

        Cell* target = getCell(cellId);
        if (!target) {
            cout << "Cell not found in init: " << cellId << endl;
            continue;
        }

        char ownerChar = toupper(ownerPart.empty() ? ' ' : ownerPart[0]);
        PlayerId owner = (ownerChar == 'A') ? PlayerId::PLAYER1 :
                             (ownerChar == 'B') ? PlayerId::PLAYER2 :
                             PlayerId::NONE;

        if (right == "Mark") {
            target->markVisible(PlayerId::PLAYER1);
            target->markVisible(PlayerId::PLAYER2);
        }
        else if (right == "Control") {
            target->controlledBy = owner;
        }
        else {
            // Unit: Scout / Sniper / Seargeant (note spelling in sample)
            AgentType at = AgentType::SCOUT;
            if (right == "Sniper")    at = AgentType::SNIPER;
            else if (right == "Seargeant" || right == "Sergeant") at = AgentType::SERGEANT;

            Agent* unit = new Agent(at, owner);
            target->agents.push_back(unit);
            unit->position = target;
        }
    }

    initf.close();
    cout << "Initial state (agents/marks/controls) applied" << endl;

    precomputeNeighbors();
}

void Board::precomputeNeighbors() {
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell* cell = getCell(r, c);
            if (!cell) continue;

            cell->neighbors.clear();

            // Always: up and down (vertical)
            if (r > 0) {
                Cell* n = getCell(r-1, c);
                if (n) cell->neighbors.push_back(n);
            }
            if (r < rows-1) {
                Cell* n = getCell(r+1, c);
                if (n) cell->neighbors.push_back(n);
            }

            // Left and right (horizontal)
            if (c > 0) {
                Cell* n = getCell(r, c-1);
                if (n) cell->neighbors.push_back(n);
            }
            if (c < cols-1) {
                Cell* n = getCell(r, c+1);
                if (n) cell->neighbors.push_back(n);
            }

            // Diagonal directions based on row parity (offset layout)
            bool evenRow = (r % 2 == 0);

            if (evenRow) {
                // Even rows: northeast and southeast
                if (r > 0 && c < cols-1) {
                    Cell* n = getCell(r-1, c+1);
                    if (n) cell->neighbors.push_back(n);
                }
                if (r < rows-1 && c < cols-1) {
                    Cell* n = getCell(r+1, c+1);
                    if (n) cell->neighbors.push_back(n);
                }
            } else {
                // Odd rows: northwest and southwest
                if (r > 0 && c > 0) {
                    Cell* n = getCell(r-1, c-1);
                    if (n) cell->neighbors.push_back(n);
                }
                if (r < rows-1 && (c > 0)) {
                    Cell* n = getCell(r+1, c-1);
                    if (n) cell->neighbors.push_back(n);
                }
            }
        }
    }
}

Cell* Board::getCell(int r, int c) const {
    if (r < 0 || r >= rows || c < 0 || c >= cols) return nullptr;
    return grid[r][c];
}

Cell* Board::getCell(const string& id) const {
    auto it = byId.find(id);
    return (it != byId.end()) ? it->second : nullptr;
}

vector<Cell*> Board::getAllCells() const {
    vector<Cell*> all;
    for (const auto& row : grid) {
        for (Cell* cell : row) {
            if (cell) all.push_back(cell);
        }
    }
    return all;
}

vector<Cell*> Board::getNeighbors(Cell* cell) const {
    return cell ? cell->neighbors : vector<Cell*>{};
}

void Board::print() const {
    cout << "\nBoard preview (" << rows << "×" << cols << "):\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell* cell = getCell(r, c);
            if (cell) {
                cout << cell->id << ":" << cell->terrain;
                if (!cell->agents.empty()) cout << " [" << cell->agents[0]->toString() << "]";
                cout << "\t";
            } else {
                cout << ".\t";
            }
        }
        cout << "\n";
    }
    cout << endl;
}
