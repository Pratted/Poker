#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QTcpSocket>

#include <vector>

#include "packet.h"
#include "player.h"
#include "mainmenu.h"

class MainMenu;
class GameUI;
class Player;

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

    bool connected();
    bool hasValidHost();
    bool connectTcp();

    void startClient();
signals:

public slots:
    void readPacket();
    bool joinGame();

private:
    QTcpSocket* socket;

    MainMenu* menu;
    GameUI* game;
    Player* player;

    QString host = "";
    quint16 port = 0;

    bool isIPv4(std::string ip);
    void loadHost();
};

#endif // CLIENT_H
