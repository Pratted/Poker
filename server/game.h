#ifndef GAME_H
#define GAME_H

#include <QObject>

#include <map>

#include "dealer.h"
#include "player.h"

class Game : public QThread
{
    Q_OBJECT
public:
    Game(int size, int chipcount, QObject* parent = 0);

    Dealer dealer;

    std::vector<Player*> players;
    std::map<int,bool> seats;

    void run();

    const int size;
    const int chipcount;
};

#endif // GAME_H
