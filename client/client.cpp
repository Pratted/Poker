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

//My old Leetcode solution to verify ipv4.
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
    host = "192.168.1.14";
    port = 45454;
}

void Client::startClient(){
    player = new Player(this),
    menu = new MainMenu;
    game = new GameUI;

    player->socket = this->socket;
    game->self = player;

    connect(socket, &QTcpSocket::readyRead, this, &Client::readPacket);
    connect(menu->ui->buttonJoinGame, &QPushButton::clicked, this, &Client::joinGame);

    menu->show();
}

void Client::readPacket(){
    socket->waitForBytesWritten(10);

    while(socket->bytesAvailable()){
        //Packet incoming;
        //socket->read(PACKET_SIZE) >> incoming;

        Packet incoming;
        incoming = socket->read(PACKET_SIZE); // >> incoming;

        incoming.unpack();

        int i = 0;
        QString str1 = "";
        QString str2 = "";

        switch(incoming.opcode){

            case Packet::Opcode::S2C_GAME_INFO:
                qDebug() << "Received Game Info!";
                menu->updateGames(incoming.payload);
                break;
            case Packet::Opcode::S2C_MESSAGE:
                qDebug() << "Got a chat message.";
                this->game->displayMessage(incoming.dest, incoming.payload);
                break;
            case Packet::Opcode::S2C_JOIN_GAME:
                game->addSelf(incoming.payload);
                break;
            case Packet::Opcode::S2C_ADD_PLAYER:
                qDebug() << "The dealer is telling us a player has joined the game.";
                game->addPlayer(incoming.payload);
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

    this->game->gameinfo.chipcount = menu->table.widget->item(gameno, COL_CHIP_COUNT)->text();
    this->game->gameinfo.maxplayers = menu->table.widget->item(gameno, COL_MAX_PLAYERS)->text();

    Packet outgoing(Packet::Opcode::C2S_JOIN_GAME, QString::number(gameno));
    socket->write(outgoing.package());

    //give the server a second to move the connection.
    socket->waitForBytesWritten(1000);

    game->show();
    return true;
}
