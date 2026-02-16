#ifndef CARD_H
#define CARD_H

#include "Types.h"

using namespace std;

struct Card {
    ActionType type;

    Card(ActionType t) : type(t) {}

    string toString() const;
};

#endif // CARD_H
