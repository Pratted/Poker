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

    table.widget->insertColumn(0);
    table.widget->insertColumn(1);
    table.widget->insertColumn(2);
    table.widget->insertColumn(3);
    table.widget->insertColumn(4);
    table.widget->insertColumn(5);

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

        table.widget->item(row, 1)->setText(game.players);
        table.widget->item(row, 2)->setText(game.maxplayers);
        table.widget->item(row, 3)->setText(game.chipcount);
        table.widget->item(row, 4)->setText("5000");
        table.widget->item(row, 5)->setText(game.status);
    }
}

void MainMenu::Table::addRow(int row){
    this->widget->insertRow(row);

    this->games[row] = new QRadioButton(this->widget);
    this->widget->setCellWidget(row,0, this->games[row]);

    qDebug() << "Create button at location: " << this->games[row];
    qDebug() << "Create button at location: " << this->widget->cellWidget(row, 0);


    //std::cout << "created button at location: " << this->widget->item(row,0);

    for(int i = 1; i < this->widget->columnCount(); i++){
        this->widget->setItem(row, i, new QTableWidgetItem(QString::number(i)));
    }
}
