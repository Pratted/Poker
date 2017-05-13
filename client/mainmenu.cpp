#include <QAbstractItemModel>
#include <QStandardItemModel>
#include <QTableWidget>
#include <QRadioButton>
#include <QString>

#include "mainmenu.h"
#include "ui_mainmenu.h"
#include "packet.h"
#include "gameinfo.h"

MainMenu::MainMenu(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainMenu)
{
    ui->setupUi(this);

    table.widget = ui->tableWidget;
    table.headers << "" << "Players" << "Max Players" << "Chip Count" << "Blinds" << "Status";

    table.widget->insertColumn(COL_SELECT_BUTTON);
    table.widget->insertColumn(COL_CURRENT_PLAYERS);
    table.widget->insertColumn(COL_MAX_PLAYERS);
    table.widget->insertColumn(COL_CHIP_COUNT);
    table.widget->insertColumn(COL_BLINDS);
    table.widget->insertColumn(COL_STATUS);

    table.widget->setHorizontalHeaderLabels(table.headers);
}

MainMenu::~MainMenu()
{
    delete ui;
}

void MainMenu::updateGames(QString gameinfo){
    int rows = table.model.rowCount();

    qDebug() << "Received: " << gameinfo;

    for(int i = 0; i < gameinfo.size(); i += 20){
        GameInfo game(gameinfo.mid(i, 20));
        /*
        QString gameno = info.mid(0,2);
        QString players = info.mid(2,1);
        QString maxplayers = info.mid(3,1);
        QString chipcount = info.mid(4,16);
        */

        if(!game) break;

        int row = game.gameno.toInt();
        if(table.games.find(row) == table.games.end()){
            qDebug() << "Adding row.";
            table.addRow(row);
        }

        table.widget->item(row, COL_CURRENT_PLAYERS)->setText(game.players);
        table.widget->item(row, COL_MAX_PLAYERS)->setText(game.maxplayers);
        table.widget->item(row, COL_CHIP_COUNT)->setText(game.chipcount);
        table.widget->item(row, COL_BLINDS)->setText("5000");
        table.widget->item(row, COL_STATUS)->setText(game.status);
    }
}

void MainMenu::Table::addRow(int row){
    this->widget->insertRow(row);

    this->games[row] = new QRadioButton(this->widget);
    this->widget->setCellWidget(row,COL_SELECT_BUTTON, this->games[row]);

    qDebug() << "Create button at location: " << this->games[row];
    qDebug() << "Create button at location: " << this->widget->cellWidget(row, COL_SELECT_BUTTON);


    //std::cout << "created button at location: " << this->widget->item(row,0);

    for(int i = 1; i < this->widget->columnCount(); i++){
        this->widget->setItem(row, i, new QTableWidgetItem(QString::number(i)));
    }
}
