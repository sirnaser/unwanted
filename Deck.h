#ifndef DECK_H
#define DECK_H

#include "Card.h"
#include <vector>
#include <deque>
#include <random>
#include <algorithm>

using namespace std;

class Deck {
private:
    deque<Card> drawPile;      // Main draw pile (shuffled)
    vector<Card> hand;         // Current player's hand
    vector<Card> discardPile;  // Discarded cards (will be shuffled back when draw empty)

public:
    Deck();
    void initDefault();
    void shuffle();

    Card draw();
    void discardToBottom(const Card& card);
    void addToDraw(const Card& card);

    const vector<Card>& getHand() const { return hand; }
    size_t getHandSize() const { return hand.size(); }

    bool isEmpty() const { return drawPile.empty() && hand.empty() && discardPile.empty(); }
    void printStatus() const;
};

#endif // DECK_H
