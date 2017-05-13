#ifndef GAMEUI_H
#define GAMEUI_H

#include <QMainWindow>
#include <QTimer>
#include <vector>

#include "seat.h"
#include "player.h"
#include "countdowntimer.h"
#include "community.h"
#include "gameinfo.h"

namespace Ui {
class GameUI;
}

class GameUI : public QMainWindow
{
    Q_OBJECT
public:
    explicit GameUI(QWidget *parent = 0);

    Ui::GameUI* ui;
    Player* self;

    std::vector<Seat> seats;
    std::vector<Player*> players;
    Community community;
    GameInfo gameinfo;

    void setBackground();
    void positionSeats();
    void displayMessage(QString sender, QString message);
    void addPlayer(QString contents);
    void addSelf(QString contents);

    void showBetMenu();

signals:

public slots:
    void chooseSeat(int seatid);
    void sendMessage();
    void startRoundTimer();
    void countdown();

private:   
    void hideBetMenu();

    QTimer nextRoundTimer;
};

#endif // GAMEUI_H
