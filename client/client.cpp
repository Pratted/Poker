#include <QFile>
#include <QMessageBox>
#include <QRadioButton>
#include <QDebug>
#include <QThread>

#include <string>

#include "client.h"
#include "gameui.h"
#include "mainmenu.h"
#include "ui_mainmenu.h"

Client::Client(QObject *parent) :
    QObject(parent),
    socket(new QTcpSocket(this))
{
    loadHost();
}

bool Client::connected(){
    return this->socket->state() == QTcpSocket::ConnectedState;
}

bool Client::connectTcp(){
    socket->connectToHost(host, port);
    return socket->waitForConnected(5000);
}

bool Client::hasValidHost(){
    qDebug() << "Host: " << host;
    qDebug() << "Port: " << port;
    return isIPv4(host.toStdString()) && (port > 0 && port < 65536);
}

bool Client::isIPv4(std::string ip){
    int dots = 0;
    for(int i = 0; i < ip.length(); i++){
        std::string val;
        while(i < ip.length() && ip[i] != '.'){
            if(!isdigit(ip[i])){ return false; }

            val+= ip[i];
            i++;

            if(val.length() > 3) { return false; }
            if(val.length() >= 2 && val[0] == '0'){ return false; }
        }

        if(val.length() == 0) return false;

        if(stoi(val) >255) return false;
        if(i < ip.length() && ip[i] == '.') dots++;
        if(dots > 3) return false;
    }

    if(dots != 3) return false;

    qDebug() << "Host.config has a valid ip.";
    return true;
}

void Client::loadHost(){
    /*
    QFile file("host.config");

    QTextStream input(&file);
    QString info = input.readLine();

    //invalid format.
    if(info.split(':').size() != 2) return;

    host = info.split(':')[0];
    port = QString(info.split(':')[1]).toUInt();
    */
    host = "192.168.1.13";
    port = 45454;
}

void Client::startClient(){
    player = new Player(this),
    menu = new MainMenu;
    game = new GameUI;

    player->socket = this->socket;
    game->player = player;

    connect(socket, &QTcpSocket::readyRead, this, &Client::readPacket);
    connect(menu->ui->buttonJoinGame, &QPushButton::clicked, this, &Client::joinGame);

    menu->show();
}

void Client::readPacket(){
    socket->waitForBytesWritten(10);

    while(socket->bytesAvailable()){
        Packet incoming;
        socket->read(PACKET_SIZE) >> incoming;

        incoming.unpack();

        switch(incoming.opcode){

            case Packet::Opcode::S2C_GAME_INFO:
                qDebug() << "Received Game Info!";
                menu->updateGames(incoming.payload);
                break;

            case Packet::Opcode::S2C_MESSAGE:
                this->game->displayMessage(incoming.payload);
                break;
            case Packet::Opcode::S2C_ADD_PLAYER:
                qDebug() << "The dealer is telling us a player has joined the game.";
                game->seats[incoming.payload.toInt()].addPlayer(this->player);
                break;
        }

        qDebug() << "Opcode: " << int(incoming.opcode);
        qDebug() << "Payload: " << incoming.payload;
    }
}

bool Client::joinGame(){
    qDebug() << "Attemping to join game.";

    int gameno = -1;
    QString status;

    for(int i = 0; i < menu->table.widget->rowCount(); i++){
        qDebug() << menu->table.widget->item(0,1)->text();

        if(menu->table.games[i]->isChecked()){
            gameno = i;
            status = menu->table.widget->item(i,5)->text();
            break;
        }
    }

    if(gameno < 0){ QMessageBox::information(0, "Cannot join game", "Select a game to join."); return false; }
    if(status != "Waiting for players"){ QMessageBox::information(0, "Cannot join game", "You cannot join an existing game."); return false; }

    Packet outgoing(Packet::Opcode::C2S_JOIN_GAME, QString::number(gameno));
    socket->write(outgoing.package());

    //give the server a second to move the connection.
    socket->waitForBytesWritten(1000);

    game->show();
    //qDebug() << "Sleeping for 5 seconds";
    //QThread::sleep(2);
    //qDebug() << "Finished";

    Packet outgoing2(Packet::Opcode::C2S_MESSAGE, "I Just joined the game!");
    socket->write(outgoing2.package());
    socket->waitForBytesWritten(1000);

    //qDebug() << "Sleeping for 5 seconds";
    //QThread::sleep(2);
    //qDebug() << "Finished";

    Packet lol(Packet::Opcode::C2S_MESSAGE, "haha");

    socket->write(lol.package());
    socket->waitForBytesWritten(1000);

    return true;
}
