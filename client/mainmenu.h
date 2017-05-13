#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QRadioButton>
#include <map>

#define COL_SELECT_BUTTON 0
#define COL_CURRENT_PLAYERS 1
#define COL_MAX_PLAYERS 2
#define COL_CHIP_COUNT 3
#define COL_BLINDS 4
#define COL_STATUS 5

namespace Ui {
class MainMenu;
}

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = 0);

    MainMenu(MainMenu &) = delete;
    MainMenu& operator=(const MainMenu& rhs) = delete;

    ~MainMenu();

    Ui::MainMenu *ui;

    struct Table {
        QTableWidget* widget;
        QStandardItemModel model;
        QStringList headers;
        std::map<int, QRadioButton*> games;

        void addRow(int pos);
    } table;

    void updateGames(QString gameinfo);
};

#endif // MAINMENU_H
