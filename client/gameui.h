#ifndef GAMEUI_H
#define GAMEUI_H

#include <QMainWindow>
#include <vector>
#include "seat.h"
#include "player.h"

namespace Ui {
class GameUI;
}

class GameUI : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameUI(QWidget *parent = 0);

    Ui::GameUI* ui;
    Player* player;

    std::vector<Seat> seats;

    void setBackground();
    void positionSeats();
    void displayMessage(QString message);

signals:

public slots:
    void chooseSeat(int seatid);
    void sendMessage();

private:
    void seatPlayer(int seatid);

};

#endif // GAMEUI_H
