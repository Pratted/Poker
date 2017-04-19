#ifndef DEALER_H
#define DEALER_H

#include <QObject>
#include <QDebug>
#include <vector>
#include <map>

#include "server.h"

class Server;
class Player;

class Dealer : public Server
{
    Q_OBJECT
public:
    Dealer(std::vector<Player*> players, std::map<int,bool> seats, QObject *parent = 0);

public slots:
    void readPacket(qintptr handle);
    void doStuff(){qDebug() << "Do Stuff called!";}

    //Dealer::~Dealer(){}

private:
    bool seatClient(qintptr handle, int seatid);

    std::vector<Player*> &players; //reference to the game's players.
    std::map<int, bool> &seats; //map to check if seats are available.
};

#endif // DEALER_H
