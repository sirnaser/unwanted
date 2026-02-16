#ifndef CARD_H
#define CARD_H

#include "Types.h"
#include <string>

using namespace std;

struct Card {
    ActionType type;
    Card(ActionType t) : type(t) {}

    bool operator==(const Card& other) const {
        return type == other.type;
    }
    string toString() const;
};

#endif // CARD_H
