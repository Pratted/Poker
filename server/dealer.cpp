#include <QDebug>

#include "packet.h"
#include "dealer.h"

Dealer::Dealer(std::vector<Player*> players, std::map<int,bool> seats, QObject* parent):
    Server(parent),
    players(players),
    seats(seats)
{
    this->start();
}

void Dealer::readPacket(qintptr handle){

    qDebug() << "Dealer received a packet from: " << handle;

    qDebug() << "Socket Address: " << clients[handle];

    while(clients[handle]->bytesAvailable()){
        qDebug() << "Dealer: Reading packet...";
        Packet incoming;
        incoming = clients[handle]->read(PACKET_SIZE); // >> incoming;

        incoming.unpack();

        switch(incoming.opcode){

            case Packet::Opcode::S2C_GAME_INFO:
                qDebug() << "Dealer: Received Game Info!";
                break;

            case Packet::Opcode::C2S_MESSAGE:
                qDebug() << "Dealer: Chat message!";
                break;

            case Packet::Opcode::C2S_JOIN_GAME:
                qDebug() << "Dealer: A client has requested to join a game!";
                //moveClientToGame(handle, incoming.payload.toInt());
                break;
            case Packet::Opcode::C2S_CHOSE_SEAT:
                qDebug() << "The client has requested a seat!";
                seatClient(handle, incoming.payload.toInt());
                break;
            default:
                qDebug() << "Uknown packet type.";
                break;
        }
    }
}

bool Dealer::seatClient(qintptr handle, int seatid){
    //Packet outgoing;

    if(seats[seatid]){
        qDebug() << "Seat " << seatid << "is available! ";

        Player* player = new Player(seatid);
        player->socket = clients[handle]->socket;

        players.push_back(player);

    }
    else{
        qDebug() << "Seat " << seatid << " is taken!";
    }


    /*
    //Seat is available. Place client in seat. Create a player.
    if(!seats[seatid].isTaken){


    }
    else{
        clients[handle]->write("Fuck you. You cannot this game!");
    }
    */
    return true;
}

