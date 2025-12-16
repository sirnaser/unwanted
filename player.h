#ifndef PLAYER_H
#define PLAYER_H

#include <QString>

class Player
{
public:
    Player(const QString &name);

    QString getName() const;

private:
    QString name;
};

#endif
