#include "Player.h"

Player::Player(const QString &name) : name(name) {}

QString Player::getName() const {
    return name;
}
