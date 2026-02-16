#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Board.h"
#include "Player.h"
#include "Config.h"
#include "Types.h"
#include <queue>
#include <unordered_map>

using namespace std;

class GameEngine {
private:
    vector<Player> players;
    Board board;
    int currentTurn = 0;
    WinCondition winCond;

    struct BFSNode {
        Cell* cell;
        int dist;
        vector<Cell*> path;
    };

    vector<Cell*> bfsShortestPath(Cell* start, Cell* end, PlayerId mover);

    int calculateTerrainSum(const vector<Cell*>& path, Agent* target);

public:
    GameEngine();

    void init(const string& boardFile = "resources/boards/board1.txt",
              const string& configFile = "resources/config.txt");

    void startGame();

    void nextTurn();

    bool performAction(ActionType action, Cell* fromCell, Cell* toCell = nullptr);

    bool checkWin() const;

    Player& getCurrentPlayer() { return players[currentTurn]; }
    const Player& getOpponent() const { return players[1 - currentTurn]; }
    const Board& getBoard() const { return board; }
    int getCurrentTurn() const { return currentTurn; }

    void printBoard() const;
};

#endif // GAMEENGINE_H
