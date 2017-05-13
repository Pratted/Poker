#include <QString>
#include <QDebug>
#include <QTcpSocket>

#include <string>

#include "player.h"
#include "packet.h"

Player::Player(QObject *parent) : QObject(parent), socket(new QTcpSocket(this))
{
    //connect(socket, &QTcpSocket::readyRead, this, &Player::readPacket);
}


