#include "Deck.h"
#include <iostream>

Deck::Deck() {}

void Deck::initDefault() {
    for (int i = 0; i < 4; ++i) {
        drawPile.emplace_back(ActionType::SCOUT);
    }
    for (int i = 0; i < 3; ++i) {
        drawPile.emplace_back(ActionType::ATTACK);
    }
    for (int i = 0; i < 3; ++i) {
        drawPile.emplace_back(ActionType::CONTROL);
    }
    shuffle();
}

void Deck::shuffle() {
    if (drawPile.empty() && !discardPile.empty()) {
        drawPile.insert(drawPile.end(), discardPile.begin(), discardPile.end());
        discardPile.clear();
    }

    random_device rd;
    mt19937 g(rd());
    std::shuffle(drawPile.begin(), drawPile.end(), g);
}

Card Deck::draw() {
    if (drawPile.empty()) {
        if (discardPile.empty()) {
            return Card(ActionType::MOVE); // fallback
        }
        shuffle();
    }

    Card drawn = drawPile.front();
    drawPile.pop_front();
    hand.push_back(drawn);
    return drawn;
}

void Deck::discardToBottom(const Card& card) {
    auto it = find(hand.begin(), hand.end(), card);
    if (it != hand.end()) {
        hand.erase(it);
    }
    drawPile.push_back(card);
}

void Deck::addToDraw(const Card& card) {
    drawPile.push_back(card);
}

void Deck::printStatus() const {
    cout << "Deck: Draw=" << drawPile.size()
    << " Hand=" << hand.size()
    << " Discard=" << discardPile.size() << endl;
}
