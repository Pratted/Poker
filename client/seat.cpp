#include <QImage>
#include <QPixmap>

#include "seat.h"
#include "player.h"

Seat::Seat(int id) :
    id(id),
    chipStacks(8)
{
    for (auto &stack : chipStacks) {
        //stack.reset(new QLabel(nullptr));
        //stack->hide();
    }
}

void Seat::clear() {
    placeholder->hide();
    buttonSitInSeat->show();
    playerName->setText("");
    chipCount->setText("");
    playerName->hide();
    chipCount->hide();

    if (isTaken) {
        labelCard1->hide();
        labelCard2->hide();

        //this->timer->hide();
        //player->stopTimer();
        //player.reset();
    }

    isTaken = false;
}

void Seat::display() {
    this->isTaken = true;
    this->buttonSitInSeat->setVisible(false);
    this->placeholder->show();

    //this->playerName->setText(player->getName());
    this->playerName->raise();
    this->playerName->show();

    this->chipCount->show();
}

void Seat::addPlayer(){
    player = new Player(this->id);

    player->labelChipCount = this->chipCount;

    player->card1.label = this->labelCard1;
    player->card2.label = this->labelCard2;

    this->isTaken = true;


    //seat.player->seat = &seat;
    //seat.player->card1.image = seat.labelCard1;
    //seat.player->card2.image = seat.labelCard2;
    //seat.player->chipCount = settings.chipStartCount;
    //player->labelChipCount = seat.chipCount;

    this->display();
}

void Seat::addPlayer(Player* player){
    player->labelChipCount = this->chipCount;

    player->card1.label = this->labelCard1;
    player->card2.label = this->labelCard2;

    this->isTaken = true;


    //seat.player->seat = &seat;
    //seat.player->card1.image = seat.labelCard1;
    //seat.player->card2.image = seat.labelCard2;
    //seat.player->chipCount = settings.chipStartCount;
    //player->labelChipCount = seat.chipCount;

    this->display();
}
