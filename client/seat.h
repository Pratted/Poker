#ifndef SEAT_H
#define SEAT_H

#include <QPicture>
#include <QLabel>
#include <QPushButton>
#include <QTimer>

#include <iostream>
#include <vector>

class Player;

class Seat {
    friend class Player;
public:
    Seat(int id);

    Player* player;

    int id;
    void clear();
    void display();

    bool isTaken = false;

    QPushButton* buttonSitInSeat;
    QLabel* playerName;
    QLabel* chipCount;
    QLabel* labelCard1;
    QLabel* labelCard2;
    QLabel* placeholder;

    QLabel* labelTimer;
    //std::unique_ptr<CountdownTimer> timer;
    //CountdownTimer timer;

    std::vector<QLabel*> chipStacks;

    void addPlayer();
    void addPlayer(Player* player);
};

#endif // SEAT_H
