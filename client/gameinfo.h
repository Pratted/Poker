#ifndef GAMEINFO_H
#define GAMEINFO_H


struct GameInfo {
    GameInfo() = default;
    GameInfo(QString info);
    GameInfo(QString, QString, QString, QString, QString);

    QString gameno;
    QString status;
    QString players;
    QString maxplayers;
    QString chipcount;

    QString package();

    bool operator!() {return this->gameno == "\0";}
};

#endif // GAMEINFO_H
