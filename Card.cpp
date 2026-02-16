#include "Card.h"

string Card::toString() const {
    switch (type) {
    case ActionType::MOVE:     return "Move";
    case ActionType::ATTACK:   return "Attack";
    case ActionType::SCOUT:    return "Scout";
    case ActionType::CONTROL:  return "Control";
    case ActionType::RELEASE:  return "Release";
    default:                   return "Unknown";
    }
}
