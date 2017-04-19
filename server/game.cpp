#include <QDebug>

#include "game.h"

Game::Game(int size, int chipcount, QObject* parent):
    size(size),
    chipcount(chipcount),
    dealer(players, seats),
    QThread(parent)
{
    qDebug() << "Game thread initialized!";

    //Make 8 open seats.
    for(int i = 0; i < 8; i++){
        seats[i] = true;
    }
}

void Game::run(){



    this->exec();
}
