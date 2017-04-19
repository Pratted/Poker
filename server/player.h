#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <QTcpSocket>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(int id, QObject *parent = 0);

    const int id;
    QTcpSocket* socket;


};

#endif // PLAYER_H
