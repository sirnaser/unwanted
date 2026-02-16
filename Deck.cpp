#include "Deck.h"
#include <iostream>

Deck::Deck() {}

void Deck::initDefault() {
    for (int i = 0; i < 4; ++i) {
        drawPile.emplace_back(ActionType::SCOUT);
    }
    for (int i = 0; i < 3; ++i) {
        drawPile.emplace_back(ActionType::SNIPER);
    }
    for (int i = 0; i < 3; ++i) {
        drawPile.emplace_back(ActionType::SERGEANT);
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
            return Card(ActionType::MOVE); // fallback dummy
        }
        shuffle(); // moves discard → draw
    }

    Card drawn = drawPile.front();
    drawPile.pop_front();
    hand.push_back(drawn);
    return drawn;
}

void Deck::discardToBottom(const Card& card) {
    // Remove from hand (assuming it's there)
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
    cout << "Deck status:\n";
    cout << "  Draw pile: " << drawPile.size() << "\n";
    cout << "  Hand: " << hand.size() << " cards\n";
    cout << "  Discard: " << discardPile.size() << "\n";

    if (!hand.empty()) {
        cout << "  Hand content: ";
        for (const auto& c : hand) {
            cout << c.toString() << " ";
        }
        cout << "\n";
    }
}
