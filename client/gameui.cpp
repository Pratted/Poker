#include <QDebug>
#include <functional>
#include <QInputDialog>

#include "countdowntimer.h"
#include "gameui.h"
#include "seat.h"
#include "ui_gameui.h"
#include "packet.h"

#define TABLE_SIZE 8
#define DEALER 9

GameUI::GameUI(QWidget *parent) : QMainWindow(parent), ui(new Ui::GameUI)
{
    ui->setupUi(this);
    this->hide();

    for(int i = 0; i < TABLE_SIZE; i++){
        seats.push_back(Seat(i));
    }

    this->setBackground();
    this->positionSeats();

    for (int i = 0; i < TABLE_SIZE; i++) {
        connect(seats[i].buttonSitInSeat, &QPushButton::clicked, this, std::bind(&GameUI::chooseSeat, this, i));
    }

    connect(ui->messageBox, &QLineEdit::returnPressed, this, &GameUI::sendMessage);
    connect(&nextRoundTimer, &QTimer::timeout, this, &GameUI::countdown);
}
/*
GameUI::GameUI(GameSettings &settings, QWidget* parent) :
    QMainWindow(parent)
{
    log("Begin GameUI Initialization.\n");
    ui->setupUi(this);

    for (int i = 0; i < TABLE_SIZE; i++) {
        seats.push_back(Seat(i, this));
    }

    this->positionSeats();
    this->setBackground();

    for (int i = 0; i < TABLE_SIZE; i++) {
        connect(seats[i].buttonSitInSeat, &QPushButton::clicked, this, std::bind(&GameUI::chooseSeat, this, i));
    }

    connect(ui->messageBox, &QLineEdit::returnPressed, this, &GameUI::sendMessage);
    log("Finish GameUI Initialization.\n");
}


void GameUI::hideSeatButtons() {
    for (auto &seat : seats) {
        seat.buttonSitInSeat->hide();
    }
}
*/

void GameUI::addPlayer(QString contents){
    /************************************************************************************
     contents is a string with the seatid and player name delimited by a colon.

     Example.            5:Eric
    *************************************************************************************/
    int seatid = QString(contents.split(':')[0]).toInt();
    QString name = contents.split(':')[1];

    Seat& seat = seats[seatid];
    if(seat.isTaken) return;

    Player* player = new Player(seat.id, name);
    seat.player = player;
    seat.playerName->setText(name);

    player->labelChipCount = seat.chipCount;
    player->labelChipCount->setText(this->gameinfo.chipcount);

    player->card1.label = seat.labelCard1;
    player->card2.label = seat.labelCard2;

    seat.isTaken = true;
    player->isSeated = true;

    seat.display();
}

void GameUI::addSelf(QString contents){
    qDebug() << "Seating myself.";

    int seatid = QString(contents.split(':')[0]).toInt();
    QString name = contents.split(':')[1];

    Seat& seat = seats[seatid];
    if(seat.isTaken) qDebug() << "Server said seat was available, but it is not.";

    self->name = name;
    self->id = seatid;
    seat.player = self;
    seat.playerName->setText(name);

    self->labelChipCount = seat.chipCount;
    self->labelChipCount->setText(this->gameinfo.chipcount);

    self->card1.label = seat.labelCard1;
    self->card2.label = seat.labelCard2;

    seat.isTaken = true;
    self->isSeated = true;

    seat.display();

    //Hide all the other seats. Client/Self is seated.
    for (auto &seat : seats) {
        seat.buttonSitInSeat->hide();
    }
}



void GameUI::chooseSeat(int seatid){
    qDebug() << "The client has chosen seat: " << seatid;

    QString name = QInputDialog::getText(this, "Enter your name", tr("Enter your name"));

    //Canceled or Blank Name.
    if(name.isEmpty()) return;

    Packet outgoing(Packet::Opcode::C2S_CHOSE_SEAT, QString::number(seatid) + ":" + name);
    //Packet outgoing(Packet::Opcode::C2S_MESSAGE, "May I have seat" + QString::number(seatid));
    self->socket->write(outgoing.package());
}

void GameUI::displayMessage(QString sender, QString message){
    int seatid = sender.toInt();
    QString name = "";

    qDebug() << "Seat: " << seatid;
    //qDebug() << "playerName->text: " << seats[seatid].playerName->text();
    //Append name + colon if player message.
    if(seatid != DEALER) name = seats[seatid].playerName->text() + ": ";

    ui->messageBrowser->append(name + message);
}

void GameUI::sendMessage() {
    QString message = ui->messageBox->text();

    if(message.isEmpty()) return;
    ui->messageBox->clear();

    Packet packet(Packet::Opcode::C2S_MESSAGE, message);
    self->socket->write(packet.package());
}

void GameUI::showBetMenu(){
    ui->buttonCall->hide();
    ui->buttonFold->hide();
    ui->buttonBet->hide();

    ui->buttonSubmit->raise();
    ui->buttonSubmit->show();

    ui->buttonCancel->raise();
    ui->buttonCancel->show();

    ui->horizontalBetSlider->raise();
    ui->horizontalBetSlider->show();

    ui->inputBet->raise();
    ui->inputBet->show();

    ui->buttonIncrementBet->raise();
    ui->buttonDecrementBet->raise();
    ui->buttonIncrementBet->show();
    ui->buttonDecrementBet->show();
}

void GameUI::hideBetMenu() {
    ui->labelButtonHolder->hide();
    ui->labelCardHolder->hide();

    ui->buttonBet->hide();
    ui->buttonCall->hide();
    ui->buttonFold->hide();
    ui->buttonCancel->hide();
    ui->buttonSubmit->hide();
    ui->inputBet->hide();
    ui->horizontalBetSlider->hide();
}

void GameUI::startRoundTimer() {
    nextRoundTimer.start(1000);
}

void GameUI::countdown() {
    static int secondsLeft = 6;
    QString countdown = "The next round begins in: ";

    if (secondsLeft == 6) {
        QFont labelFont;
        labelFont.setPixelSize(20);
        ui->labelBottomRight->setFont(labelFont);
        secondsLeft--;
    }

    countdown += ('0' + secondsLeft);

    if (secondsLeft) {
        startRoundTimer();
        ui->labelBottomRight->setText(countdown);
        ui->labelBottomRight->show();
        secondsLeft--;
    }
    else {
        nextRoundTimer.stop();
        ui->labelBottomRight->setText("");
        ui->labelBottomRight->hide();
        secondsLeft = 5;
    }
}


void GameUI::setBackground() {

    ui->labelBottomLeft->hide();
    ui->labelBottomRight->hide();

    ui->buttonBet->hide();
    ui->buttonCall->hide();
    ui->buttonCancel->hide();
    ui->buttonFold->hide();
    ui->buttonSubmit->hide();
    ui->horizontalBetSlider->hide();
    ui->inputBet->hide();
    ui->buttonIncrementBet->hide();
    ui->buttonDecrementBet->hide();

    ui->labelButtonHolder->hide();
    ui->labelCardHolder->hide();
    ui->labelPlayerCard1->hide();
    ui->labelPlayerCard2->hide();

    ui->labelCommunityCard1->hide();
    ui->labelCommunityCard2->hide();
    ui->labelCommunityCard3->hide();
    ui->labelCommunityCard4->hide();
    ui->labelCommunityCard5->hide();

    ui->placeholder0->hide();
    ui->placeholder6->hide();
    ui->placeholder5->hide();
    ui->placeholder7->hide();
    ui->placeholder4->hide();
    ui->placeholder3->hide();
    ui->placeholder2->hide();
    ui->placeholder1->hide();

    ui->labelPotHolder->hide();
    ui->labelPot->setText("");
    ui->label->hide();

}
void GameUI::positionSeats() {

    QPalette whiteText;
    whiteText.setColor(QPalette::WindowText, Qt::white);

    QFont labelFont;
    labelFont.setPixelSize(24);

    ui->labelBottomLeft->setPalette(whiteText);
    ui->labelBottomLeft->setFont(labelFont);

    ui->labelBottomRight->setPalette(whiteText);
    ui->labelBottomRight->setFont(labelFont);

    QFont font;
    font.setPixelSize(20);


    //Seat 0
    {
        seats[0].buttonSitInSeat = ui->buttonSitInSeat0;
        seats[0].placeholder = ui->placeholder0;
        seats[0].playerName = ui->Player0Name;
        seats[0].chipCount = ui->Player0ChipCount;
        seats[0].labelCard1 = ui->Player0Card1;
        seats[0].labelCard2 = ui->Player0Card2;
        seats[0].timer = new CountdownTimer;
        seats[0].timer->setGeometry(735, 25, 150, 150);
        seats[0].timer->hide();
        seats[0].chipStacks[0] = ui->Player0Stack0;
        seats[0].chipStacks[1] = ui->Player0Stack1;
        seats[0].chipStacks[2] = ui->Player0Stack2;
        seats[0].chipStacks[3] = ui->Player0Stack3;
        seats[0].chipStacks[4] = ui->Player0Stack4;
        seats[0].chipStacks[5] = ui->Player0Stack5;
        seats[0].chipStacks[6] = ui->Player0Stack6;
        seats[0].chipStacks[7] = ui->Player0Stack7;
    }
    //Seat 1
    {
        seats[1].buttonSitInSeat = ui->buttonSitInSeat1;
        seats[1].placeholder = ui->placeholder1;
        seats[1].playerName = ui->Player1Name;
        seats[1].chipCount = ui->Player1ChipCount;
        seats[1].labelCard1 = ui->Player1Card1;
        seats[1].labelCard2 = ui->Player1Card2;
        seats[1].timer = new CountdownTimer(ui->centralwidget);
        seats[1].timer->setGeometry(975, 175, 150, 150);
        seats[1].timer->hide();

        seats[1].chipStacks[0] = ui->Player1Stack0;
        seats[1].chipStacks[1] = ui->Player1Stack1;
        seats[1].chipStacks[2] = ui->Player1Stack2;
        seats[1].chipStacks[3] = ui->Player1Stack3;
        seats[1].chipStacks[4] = ui->Player1Stack4;
        seats[1].chipStacks[5] = ui->Player1Stack5;
        seats[1].chipStacks[6] = ui->Player1Stack6;
        seats[1].chipStacks[7] = ui->Player1Stack7;
    }
    //Seat 2
    {
        seats[2].buttonSitInSeat = ui->buttonSitInSeat2;
        seats[2].placeholder = ui->placeholder2;
        seats[2].playerName = ui->Player2Name;
        seats[2].chipCount = ui->Player2ChipCount;
        seats[2].labelCard1 = ui->Player2Card1;
        seats[2].labelCard2 = ui->Player2Card2;
        seats[2].timer = new CountdownTimer(ui->centralwidget);
        seats[2].timer->setGeometry(945, 365, 150, 150);
        seats[2].timer->hide();

        seats[2].chipStacks[0] = ui->Player2Stack0;
        seats[2].chipStacks[1] = ui->Player2Stack1;
        seats[2].chipStacks[2] = ui->Player2Stack2;
        seats[2].chipStacks[3] = ui->Player2Stack3;
        seats[2].chipStacks[4] = ui->Player2Stack4;
        seats[2].chipStacks[5] = ui->Player2Stack5;
        seats[2].chipStacks[6] = ui->Player2Stack6;
        seats[2].chipStacks[7] = ui->Player2Stack7;
    }
    //Seat 3
    {
        seats[3].buttonSitInSeat = ui->buttonSitInSeat3;
        seats[3].placeholder = ui->placeholder3;
        seats[3].playerName = ui->Player3Name;
        seats[3].chipCount = ui->Player3ChipCount;
        seats[3].labelCard1 = ui->Player3Card1;
        seats[3].labelCard2 = ui->Player3Card2;
        seats[3].timer = new CountdownTimer(ui->centralwidget);
        seats[3].timer->setGeometry(675, 455, 150, 150);
        seats[3].timer->hide();

        seats[3].chipStacks[0] = ui->Player3Stack0;
        seats[3].chipStacks[1] = ui->Player3Stack1;
        seats[3].chipStacks[2] = ui->Player3Stack2;
        seats[3].chipStacks[3] = ui->Player3Stack3;
        seats[3].chipStacks[4] = ui->Player3Stack4;
        seats[3].chipStacks[5] = ui->Player3Stack5;
        seats[3].chipStacks[6] = ui->Player3Stack6;
        seats[3].chipStacks[7] = ui->Player3Stack7;
    }
    //Seat 4
    {
        seats[4].buttonSitInSeat = ui->buttonSitInSeat4;
        seats[4].placeholder = ui->placeholder4;
        seats[4].playerName = ui->Player4Name;
        seats[4].chipCount = ui->Player4ChipCount;
        seats[4].labelCard1 = ui->Player4Card1;
        seats[4].labelCard2 = ui->Player4Card2;
        seats[4].timer = new CountdownTimer(ui->centralwidget);
        seats[4].timer->setGeometry(478, 455, 150, 150);
        seats[4].timer->hide();

        seats[4].chipStacks[0] = ui->Player4Stack0;
        seats[4].chipStacks[1] = ui->Player4Stack1;
        seats[4].chipStacks[2] = ui->Player4Stack2;
        seats[4].chipStacks[3] = ui->Player4Stack3;
        seats[4].chipStacks[4] = ui->Player4Stack4;
        seats[4].chipStacks[5] = ui->Player4Stack5;
        seats[4].chipStacks[6] = ui->Player4Stack6;
        seats[4].chipStacks[7] = ui->Player4Stack7;
    }

    //Seat 5
    {
        seats[5].buttonSitInSeat = ui->buttonSitInSeat5;
        seats[5].placeholder = ui->placeholder5;
        seats[5].playerName = ui->Player5Name;
        seats[5].chipCount = ui->Player5ChipCount;
        seats[5].labelCard1 = ui->Player5Card1;
        seats[5].labelCard2 = ui->Player5Card2;
        seats[5].timer = new CountdownTimer(ui->centralwidget);
        seats[5].timer->setGeometry(208, 365, 150, 150);
        seats[5].timer->hide();

        seats[5].chipStacks[0] = ui->Player5Stack0;
        seats[5].chipStacks[1] = ui->Player5Stack1;
        seats[5].chipStacks[2] = ui->Player5Stack2;
        seats[5].chipStacks[3] = ui->Player5Stack3;
        seats[5].chipStacks[4] = ui->Player5Stack4;
        seats[5].chipStacks[5] = ui->Player5Stack5;
        seats[5].chipStacks[6] = ui->Player5Stack6;
        seats[5].chipStacks[7] = ui->Player5Stack7;
    }

    //Seat 6
    {
        seats[6].buttonSitInSeat = ui->buttonSitInSeat6;
        seats[6].placeholder = ui->placeholder6;
        seats[6].playerName = ui->Player6Name;
        seats[6].chipCount = ui->Player6ChipCount;
        seats[6].labelCard1 = ui->Player6Card1;
        seats[6].labelCard2 = ui->Player6Card2;
        seats[6].timer = new CountdownTimer(ui->centralwidget);
        seats[6].timer->setGeometry(188, 165, 150, 150);
        seats[6].timer->hide();

        seats[6].chipStacks[0] = ui->Player6Stack0;
        seats[6].chipStacks[1] = ui->Player6Stack1;
        seats[6].chipStacks[2] = ui->Player6Stack2;
        seats[6].chipStacks[3] = ui->Player6Stack3;
        seats[6].chipStacks[4] = ui->Player6Stack4;
        seats[6].chipStacks[5] = ui->Player6Stack5;
        seats[6].chipStacks[6] = ui->Player6Stack6;
        seats[6].chipStacks[7] = ui->Player6Stack7;
    }
    //Seat 7
    {
        seats[7].buttonSitInSeat = ui->buttonSitInSeat7;
        seats[7].placeholder = ui->placeholder7;
        seats[7].playerName = ui->Player7Name;
        seats[7].chipCount = ui->Player7ChipCount;
        seats[7].labelCard1 = ui->Player7Card1;
        seats[7].labelCard2 = ui->Player7Card2;
        seats[7].timer = new CountdownTimer(ui->centralwidget);
        seats[7].timer->setGeometry(398, 25, 150, 150);
        seats[7].timer->hide();

        seats[7].chipStacks[0] = ui->Player7Stack0;
        seats[7].chipStacks[1] = ui->Player7Stack1;
        seats[7].chipStacks[2] = ui->Player7Stack2;
        seats[7].chipStacks[3] = ui->Player7Stack3;
        seats[7].chipStacks[4] = ui->Player7Stack4;
        seats[7].chipStacks[5] = ui->Player7Stack5;
        seats[7].chipStacks[6] = ui->Player7Stack6;
        seats[7].chipStacks[7] = ui->Player7Stack7;
    }

    community.card1.label = ui->labelCommunityCard1;
    community.card2.label = ui->labelCommunityCard2;
    community.card3.label = ui->labelCommunityCard3;
    community.card4.label = ui->labelCommunityCard4;
    community.card5.label = ui->labelCommunityCard5;

    for (auto &seat : seats) {
        seat.playerName->hide();
        seat.chipCount->hide();
        seat.labelCard1->hide();
        seat.labelCard2->hide();

        for (auto &stack : seat.chipStacks) {
            stack->hide();
        }
    }
}
