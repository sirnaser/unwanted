#include "GameEngine.h"
#include <iostream>
#include <random>
#include <algorithm>

string actionToString(ActionType a) {
    switch (a) {
    case ActionType::MOVE:    return "MOVE";
    case ActionType::ATTACK:  return "ATTACK";
    case ActionType::SCOUT:   return "SCOUT";
    case ActionType::CONTROL: return "CONTROL";
    case ActionType::RELEASE: return "RELEASE";
    default:                  return "UNKNOWN";
    }
}

GameEngine::GameEngine() {
    players.emplace_back("Player A", PlayerId::PLAYER1);
    players.emplace_back("Player B", PlayerId::PLAYER2);
}

void GameEngine::init(const string& boardFile, const string& configFile) {
    winCond = Config::load(configFile);
    board.load(boardFile, "resources/boards/2.txt");

    for (Cell* cell : board.getAllCells()) {
        for (Agent* agent : cell->agents) {
            if (agent->owner == PlayerId::PLAYER1) players[0].agents.push_back(agent);
            else if (agent->owner == PlayerId::PLAYER2) players[1].agents.push_back(agent);
        }
    }
}

void GameEngine::startGame() {
    for (Player& p : players) {
        p.deck.initDefault();
        p.deck.shuffle();
        for (int i = 0; i < 4; ++i) {
            if (!p.deck.drawPile.empty()) p.deck.draw();
        }
    }
    currentTurn = 0;
}

void GameEngine::nextTurn() {
    currentTurn = 1 - currentTurn;
    Player& p = getCurrentPlayer();
    p.deck.draw();
}

bool GameEngine::performAction(ActionType action, Cell* fromCell, Cell* toCell) {
    if (!fromCell || fromCell->agents.empty()) return false;

    Agent* agent = fromCell->agents[0];
    if (agent->owner != getCurrentPlayer().id) return false;

    if (action == ActionType::MOVE) {
        if (!toCell) return false;
        auto path = bfsShortestPath(fromCell, toCell, agent->owner);
        if (path.empty() || path.size() > 3) return false;

        fromCell->agents.clear();
        toCell->agents.push_back(agent);
        agent->position = toCell;
        return true;
    }

    if (action == ActionType::SCOUT) {
        if (toCell) toCell->markVisible(agent->owner);
        return true;
    }

    if (action == ActionType::ATTACK) {
        if (!toCell || toCell->agents.empty()) return false;
        Agent* target = toCell->agents[0];
        if (target->owner == agent->owner) return false;

        auto path = bfsShortestPath(fromCell, toCell, agent->owner);
        if (path.empty()) return false;

        int attackValue = target->hp;
        for (size_t i = 1; i < path.size(); ++i) {
            attackValue += path[i]->terrain;
        }

        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<int> d10(1, 10);
        int roll = d10(gen) + d10(gen) + d10(gen);

        cout << "Attack roll: " << roll << " vs " << attackValue << endl;

        if (roll >= attackValue) {
            Player& opp = getOpponent();
            ActionType expected;
            if (target->type == AgentType::SCOUT) expected = ActionType::SCOUT;
            else if (target->type == AgentType::SNIPER) expected = ActionType::ATTACK;
            else expected = ActionType::CONTROL;

            for (auto it = opp.deck.hand.begin(); it != opp.deck.hand.end(); ++it) {
                if (it->type == expected) {
                    opp.deck.discardToBottom(*it);
                    opp.deck.hand.erase(it);
                    cout << "Attack success - card removed" << endl;
                    return true;
                }
            }
            target->takeDamage(1);
            cout << "Attack hit (1 dmg)" << endl;
            return true;
        }
        cout << "Attack missed" << endl;
        return true;
    }

    cout << "Action " << actionToString(action) << " not implemented" << endl;
    return false;
}

vector<Cell*> GameEngine::bfsShortestPath(Cell* start, Cell* goal, PlayerId mover) {
    if (!start || !goal) return {};

    unordered_map<Cell*, Cell*> cameFrom;
    unordered_map<Cell*, int> dist;
    queue<Cell*> q;

    q.push(start);
    cameFrom[start] = nullptr;
    dist[start] = 0;

    while (!q.empty()) {
        Cell* curr = q.front(); q.pop();
        if (curr == goal) {
            vector<Cell*> path;
            for (Cell* at = goal; at != start; at = cameFrom[at]) {
                path.push_back(at);
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
            return path;
        }

        for (Cell* n : board.getNeighbors(curr)) {
            if (dist.find(n) == dist.end() && n->isVisibleFor(mover)) {
                q.push(n);
                cameFrom[n] = curr;
                dist[n] = dist[curr] + 1;
            }
        }
    }
    return {};
}

bool GameEngine::checkWin() const {
    for (const auto& p : players) {
        if (p.hasWon(winCond)) return true;
    }
    return false;
}

string GameEngine::getTurnStatus() const {
    return "Turn: " + getCurrentPlayer().name;
}

void GameEngine::printGameState() const {
    cout << getTurnStatus() << "\n";
}

void GameEngine::setPlayerName(int index, const string& name) {
    if (index == 0 || index == 1) {
        players[index].name = name;
    }
}
