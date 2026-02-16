#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "Board.h"
#include "Player.h"
#include "Config.h"
#include "Types.h"
#include <queue>
#include <unordered_map>
#include <vector>
#include <string>

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

    vector<Cell*> bfsShortestPath(Cell* start, Cell* goal, PlayerId mover);
    int calculateTerrainSum(const vector<Cell*>& path, Agent* target) const;

public:
    GameEngine();

    void init(const string& boardFile = "resources/boards/1.txt",
              const string& configFile = "resources/config.txt");

    void startGame();
    void nextTurn();
    bool performAction(ActionType action, Cell* fromCell, Cell* toCell = nullptr);
    bool checkWin() const;

    Player& getCurrentPlayer();
    const Player& getCurrentPlayer() const;

    Player& getOpponent();
    const Player& getOpponent() const;

    void setPlayerName(int index, const string& name);

    const Board& getBoard() const { return board; }
    int getCurrentTurnIndex() const { return currentTurn; }
    string getTurnStatus() const;

    void printGameState() const;
};

#endif // GAMEENGINE_H
