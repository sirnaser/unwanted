#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <deque>
#include <random>

using namespace std;

class Deck {
private:
    deque<Card> drawPile;
    vector<Card> hand;
    vector<Card> discardPile;

public:
    Deck();

    void initDefault();

    void shuffle();
    Card draw();
    void discard(const Card& card);
    void addToDraw(const Card& card);

    const vector<Card>& getHand() const { return hand; }
    size_t getHandSize() const { return hand.size(); }

    bool isEmpty() const { return drawPile.empty() && hand.empty(); }
};

#endif // DECK_H
