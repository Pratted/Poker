#include <QString>

#include "gameinfo.h"

GameInfo::GameInfo(QString info){
    gameno = info.mid(0,1);
    status = info.mid(1,1);
    players = info.mid(2,1);
    maxplayers = info.mid(3,1);
    chipcount = info.mid(4,16);

    switch(info[1].toLatin1()){
    case 'W':
        status = "Waiting for players";
        break;
    case 'I':
        status = "In Progress";
        break;
    case 'S':
        status = "Starting";
        break;
    }
}

GameInfo::GameInfo(QString num, QString status, QString players, QString maxplayers, QString chipcount):
    gameno(num),
    status(status),
    players(players),
    maxplayers(maxplayers),
    chipcount(chipcount)
{
    switch(status[0].toLatin1()){
    case 'W':
        this->status = "Waiting for players";
        break;
    case 'I':
        this->status = "In Progress";
        break;
    case 'S':
        this->status = "Starting";
        break;
    }
}


QString GameInfo::package(){
    return gameno + status.mid(0,1) + players + maxplayers + chipcount.leftJustified(16, '\0');
}
