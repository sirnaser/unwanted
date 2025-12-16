#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include "player.h"

class GameManager
{
public:
    GameManager(const Player &p1, const Player &p2);

private:
    Player player1;
    Player player2;
};

#endif
