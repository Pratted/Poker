#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QThread>
#include <QTimer>

#include <vector>
#include <unordered_map>

#include "player.h"

class Game;
class Player;

class Server : public QTcpServer
{
    Q_OBJECT
public:
    Server(QObject* parent = 0);
    //~Server(){}

    bool start();
    bool loadGames();

    std::unordered_map<int, QTcpSocket*> clients;
    std::unordered_map<int, QMetaObject::Connection> connections;

public slots:
    void readPacket(qintptr handle);
    void moveClientToGame(qintptr handle, int gameno);
    void handleDisconnect();

    virtual void messageClients();

protected:
    void incomingConnection(qintptr handle);

private:
    QTimer timer;

    std::vector<Game*> games;
    std::unordered_map<int, Player*> clients2;

    void sendGameInfo(qintptr handle); // Send Game Info for every game. (used for new connections).
    void sendGameInfo(qintptr handle, int gameno); //Send game info for one game. (used if one game is updated).
};

#endif // SERVER_H
