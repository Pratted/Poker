#include <QObject>
#include <QDebug>
#include <functional>
#include <iostream>

#include "server.h"
#include "player.h"
#include "game.h"
#include "packet.h"
#include "gameinfo.h"

Server::Server(QObject* parent): QTcpServer(parent), timer(this)
{
    if(this->listen(QHostAddress::AnyIPv4, 45454)){
        qDebug() << "Server started.";
    }
    else{
        qDebug() << "Server failed to start.";
    }

    games.push_back(new Game(2,this));

    connect(games[0], SIGNAL(finished()), games[0], SLOT(deleteLater()));
    std::cout << "Constructed\n";


    timer.start(5000);

    connect(&timer, &QTimer::timeout, this, &Server::messageClients);
}

void Server::incomingConnection(qintptr handle){
    std::cout << "Incoming connection!\n";
    qDebug() << "A player has connected.";
    clients[handle] = new Player(handle, this);
    clients[handle]->socket = new QTcpSocket(clients[handle]);

    clients[handle]->socket->setSocketDescriptor(handle);
    connect(clients[handle]->socket, &QTcpSocket::readyRead, this, std::bind(&Server::readPacket, this, handle));

    Packet outgoing(Packet::Opcode::S2C_MESSAGE, "Hello World");

    //QByteArray arr = outgoing.package();

    //qDebug() << "arr size: " << arr.size();

    //std::cout << "Payload: " << outgoing.payload.toStdString() << std::endl;



    clients[handle]->socket->write(outgoing.package());

    //clients[handle]->socket->write("Hello\n");
    //connect(clients[handle]->socket, SIGNAL(readyRead()), this, SLOT(readPacket(handle)));
}


void Server::readPacket(qintptr handle){
    qDebug() << "Received a message from player: " << handle;

    while(clients[handle]->socket->bytesAvailable()){
        qDebug() << "Reading packet...";
        Packet incoming;
        incoming = clients[handle]->socket->read(PACKET_SIZE); // >> incoming;

        incoming.unpack();

        switch(incoming.opcode){

            case Packet::Opcode::S2C_GAME_INFO:
                qDebug() << "Received Game Info!";
                //menu->updateGames(incoming.payload);
                break;

            case Packet::Opcode::S2C_MESSAGE:
                qDebug() << "Chat message!";
                break;
            /*
            case Packet::Opcode::C2S_JOIN_GAME:
                qDebug() << "A client has requested to join a game!";
            break;
            */


        }

        qDebug() << "Opcode: " << int(incoming.opcode);
        qDebug() << "Payload: " << incoming.payload;
    }
}

void Server::messageClients(){
    std::cout << "Messaging clients.\n";

    Packet outgoing(Packet::Opcode::S2C_GAME_INFO);

    GameInfo info("0", "Waiting for Players", "0", "6", "500000");

    for(auto &client: clients){
        auto &socket = client.second->socket;
        //outgoing.payload = QString("0201500000").leftJustified(20, '\0');
        outgoing.payload = info.package();
        socket->write(outgoing.package());
    }
}

