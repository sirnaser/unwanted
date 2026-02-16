#include "Cell.h"
#include "Agent.h"

Cell::Cell(const string& _id, int _row, int _col, int _terrain)
    : id(_id), row(_row), col(_col), terrain(_terrain) {}

void Cell::markVisible(PlayerId player) {
    if (player == PlayerId::PLAYER1) visible[0] = true;
    else if (player == PlayerId::PLAYER2) visible[1] = true;
}

bool Cell::isVisibleFor(PlayerId player) const {
    if (player == PlayerId::PLAYER1) return visible[0];
    if (player == PlayerId::PLAYER2) return visible[1];
    return false;
}

string Cell::getDisplayText() const {
    if (!agents.empty()) {
        return agents[0]->toString();
    }
    if (controlledBy != PlayerId::NONE) {
        return "C";
    }
    return to_string(terrain);
}
