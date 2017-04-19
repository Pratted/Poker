#include <QObject>
#include <QDebug>
#include <functional>
#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTcpSocket>

#include <vector>

#include "server.h"
#include "dealer.h"
#include "player.h"
#include "game.h"
#include "packet.h"
#include "gameinfo.h"

Server::Server(QObject* parent): QTcpServer(parent), timer(this)
{
    std::cout << "Constructed\n";

    //timer.start(5000);

    //connect(&timer, &QTimer::timeout, this, &Server::messageClients);
}

bool Server::start(){
    return this->listen(QHostAddress::AnyIPv4, 45454);
}

bool Server::loadGames(){
    QFile file("games.json");
    file.open(QIODevice::ReadOnly);

    QString contents =file.readAll();
    file.close();

    QJsonArray loaded = QJsonDocument::fromJson(contents.toUtf8()).array();

    if(loaded.empty()) return false;

    for(int i = 0; i < loaded.size(); i++){
        QJsonObject game = loaded[i].toObject();

        int size = game["maxplayers"].toInt();
        int chipcount = game["chipcount"].toInt();

        qDebug() << "Loading game: " << i;
        qDebug() << "Size: " << size;
        qDebug() << "Chip count: " << chipcount;

        //create the game thread. Don't start it yet.
        games.push_back(new Game(size, chipcount, this));
        connect(games[i], SIGNAL(finished()), games[i], SLOT(deleteLater()));
        qDebug() << "-----------------------------";
    }
    return true;
}

void Server::incomingConnection(qintptr handle){
    std::cout << "Incoming connection!\n";
    qDebug() << "A player has connected.";
    clients[handle] = new QTcpSocket(this);

    clients[handle]->setSocketDescriptor(handle);
    connections[handle] = connect(clients[handle], &QTcpSocket::readyRead, this, std::bind(&Server::readPacket, this, handle));

    connect(clients[handle], &QTcpSocket::disconnected, this, &Server::handleDisconnect);

    this->sendGameInfo(handle);
}

void Server::handleDisconnect(){
    qDebug() << "A client has been disconnected!";
}

void Server::readPacket(qintptr handle){
    qDebug() << "Received a message from player: " << handle;

    while(clients[handle]->bytesAvailable()){
        qDebug() << "Reading packet...";
        Packet incoming;
        incoming = clients[handle]->read(PACKET_SIZE); // >> incoming;

        incoming.unpack();

        switch(incoming.opcode){

            case Packet::Opcode::S2C_GAME_INFO:
                qDebug() << "Received Game Info!";
                //menu->updateGames(incoming.payload);
                break;

            case Packet::Opcode::S2C_MESSAGE:
                qDebug() << "Chat message!";
                break;

            case Packet::Opcode::C2S_JOIN_GAME:
                qDebug() << "A client has requested to join a game!";
                moveClientToGame(handle, incoming.payload.toInt());
                break;
        }

        qDebug() << "Opcode: " << int(incoming.opcode);
        qDebug() << "Payload: " << incoming.payload;
    }
}

void Server::moveClientToGame(qintptr handle, int gameno){
    if(gameno >= games.size() || gameno < 0) return;

    qDebug() << "&socket: " << clients[handle];

    //the dealert uses the same socket as the master server
    games[gameno]->dealer.clients[handle] = clients[handle];//the game's client socket points to the global socket.

    //remove connection from master server. The socket is still live and active.
    disconnect(connections[handle]);
    //clients.erase(handle);

    qDebug() << "&socket: " << games[gameno]->dealer.clients[handle];

    //Bind the socket to the dealer. All messages will now be handled by the dealer.
    games[gameno]->dealer.connections[handle] = connect(games[gameno]->dealer.clients[handle], &QTcpSocket::readyRead, &games[gameno]->dealer, std::bind(&Dealer::readPacket, &games[gameno]->dealer, handle));

    qDebug() << "Bindng players socket to game.";
}

void Server::messageClients(){
    std::cout << "Messaging clients.\n";

    Packet outgoing(Packet::Opcode::S2C_GAME_INFO);

    GameInfo info("0", "Waiting for Players", "0", "6", "500000");

    for(auto &client: clients){
        auto &socket = client.second;
        outgoing.payload = info.package();
        //socket->write(outgoing.package());
    }

    for(auto &client: clients){


    }

}

void Server::sendGameInfo(qintptr handle){
    Packet outgoing(Packet::Opcode::S2C_GAME_INFO);

    for(int i = 0; i < games.size(); i++){
        GameInfo gameinfo(QString::number(i), "Waiting for Players", "0", QString::number(games[i]->size), QString::number(games[i]->chipcount));
        outgoing.payload += gameinfo.package();
    }

    clients[handle]->write(outgoing.package());
}

void Server::sendGameInfo(qintptr handle, int gameno){
    Packet outgoing(Packet::Opcode::S2C_GAME_INFO);

    GameInfo gameinfo(QString::number(gameno), "Waiting for Players", "0", QString::number(games[gameno]->size), QString::number(games[gameno]->chipcount));
    outgoing.payload += gameinfo.package();

    clients[handle]->write(outgoing.package());
}

