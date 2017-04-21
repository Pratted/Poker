#ifndef MAINMENU_H
#define MAINMENU_H

#include <QMainWindow>
#include <QTableWidget>
#include <QStandardItemModel>
#include <QRadioButton>
#include <map>

namespace Ui {
class MainMenu;
}

class MainMenu : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainMenu(QWidget *parent = 0);
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
