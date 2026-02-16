#include "GameEngine.h"
#include <iostream>
#include <random>
#include <algorithm>

GameEngine::GameEngine() {
    players.emplace_back("Player A", PlayerId::PLAYER1);
    players.emplace_back("Player B", PlayerId::PLAYER2);
}

void GameEngine::init(const string& boardFile, const string& initFile, const string& configFile) {
    // Load win condition
    winCond = Config::load(configFile);

    // Load board + initial agents/marks/controls
    board.load(boardFile, initFile);

    // Assign agents to players (from board)
    for (Cell* cell : board.getAllCells()) {
        for (Agent* agent : cell->agents) {
            if (agent->owner == PlayerId::PLAYER1) {
                players[0].agents.push_back(agent);
            } else if (agent->owner == PlayerId::PLAYER2) {
                players[1].agents.push_back(agent);
            }
        }
    }

    cout << "GameEngine initialized with board and win condition." << endl;
}

void GameEngine::startGame() {
    // Init and shuffle decks for both players
    for (Player& p : players) {
        p.deck.initDefault();
        p.deck.shuffle();

        // Optional: draw initial hand (e.g. 4 cards)
        for (int i = 0; i < 4; ++i) {
            if (!p.deck.drawPile.empty()) {
                p.deck.draw();
            }
        }
    }

    // Player 1 starts
    currentTurn = 0;

    cout << "Game started. " << players[0].name << " begins." << endl;
}

void GameEngine::nextTurn() {
    currentTurn = 1 - currentTurn;

    // Draw a card for the new current player
    Player& p = getCurrentPlayer();
    if (!p.deck.drawPile.empty() || !p.deck.discardPile.empty()) {
        p.deck.draw();
    }

    cout << "Turn switched to " << p.name << endl;
}

bool GameEngine::performAction(ActionType action, Cell* fromCell, Cell* toCell) {
    if (!fromCell || fromCell->agents.empty()) return false;

    Agent* agent = fromCell->agents[0];
    PlayerId mover = agent->owner;
    if (mover != getCurrentPlayer().id) return false;

    if (action == ActionType::MOVE) {
        if (!toCell || toCell == fromCell) return false;

        // Check if path exists and is visible
        vector<Cell*> path = bfsShortestPath(fromCell, toCell, mover);
        if (path.empty() || path.size() > 2) return false;  // simple limit for now

        // Move agent
        fromCell->agents.clear();
        toCell->agents.push_back(agent);
        agent->position = toCell;

        cout << agent->toString() << " moved from " << fromCell->id << " to " << toCell->id << endl;
        return true;
    }

    if (action == ActionType::SCOUT) {
        if (!toCell) return false;

        // Scout reveals cell
        toCell->markVisible(mover);

        cout << "Scouted cell " << toCell->id << endl;
        return true;
    }

    // TODO: ATTACK, CONTROL, RELEASE
    cout << "Action " << action << " not implemented yet" << endl;
    return false;
}

bool GameEngine::checkWin() const {
    for (const Player& p : players) {
        if (p.hasWon(winCond)) {
            cout << p.name << " has won!" << endl;
            return true;
        }
    }
    return false;
}

vector<Cell*> GameEngine::bfsShortestPath(Cell* start, Cell* goal, PlayerId mover) {
    if (!start || !goal || start == goal) return {};

    unordered_map<Cell*, Cell*> cameFrom;
    unordered_map<Cell*, int> dist;
    queue<Cell*> q;

    q.push(start);
    cameFrom[start] = nullptr;
    dist[start] = 0;

    while (!q.empty()) {
        Cell* current = q.front();
        q.pop();

        if (current == goal) {
            // Reconstruct path
            vector<Cell*> path;
            Cell* step = goal;
            while (step != start) {
                path.push_back(step);
                step = cameFrom[step];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        for (Cell* neighbor : board.getNeighbors(current)) {
            if (dist.find(neighbor) == dist.end() &&
                neighbor->isVisibleFor(mover)) {  // only visible cells
                q.push(neighbor);
                cameFrom[neighbor] = current;
                dist[neighbor] = dist[current] + 1;
            }
        }
    }

    return {};  // no path
}

int GameEngine::calculateTerrainSum(const vector<Cell*>& path, Agent* target) const {
    int sum = target->hp;  // base
    for (Cell* cell : path) {
        sum += cell->terrain;
    }
    return sum;
}

string GameEngine::getTurnStatus() const {
    return "Turn: " + getCurrentPlayer().name;
}

void GameEngine::printGameState() const {
    cout << "Current turn: " << getCurrentPlayer().name << endl;
    board.print();
    cout << "Player1 agents: " << players[0].agents.size() << endl;
    cout << "Player2 agents: " << players[1].agents.size() << endl;
}
