#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QTcpSocket>
#include <QLabel>

#include "card.h"

class Player : public QObject
{
    Q_OBJECT

    friend class GameUI;
    friend class Client;
public:
    Player(QObject *parent = 0);
    Player(int seatid):id(seatid){}
    Player(int seatid, QString name):id(id), name(name){}

    Player(Player &) = delete;
    Player(Player &&) = delete;
    Player& operator=(Player &rhs) = delete;
    Player& operator=(Player &&rhs) = delete;

    QTcpSocket* socket = nullptr;
    QLabel* labelChipCount;

    Card card1;
    Card card2;

    bool isSeated = false;
private:
    int id;
    QString name;
};

#endif // PLAYER_H
