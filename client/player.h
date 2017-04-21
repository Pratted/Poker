#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QTcpSocket>
#include <QLabel>

#include "card.h"

class Player : public QObject
{
    Q_OBJECT
public:
    Player(QObject *parent = 0);
    Player(int seatid):id(seatid){}

    QTcpSocket* socket;
    bool connectTcp(std::pair<QString, QString>);

    QLabel* labelChipCount;

    Card card1;
    Card card2;
private:
    int id;

};

#endif // PLAYER_H
