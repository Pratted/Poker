/********************************************************************************
** Form generated from reading UI file 'mainmenu.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINMENU_H
#define UI_MAINMENU_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainMenu
{
public:
    QWidget *centralWidget;
    QPushButton *buttonBack;
    QPushButton *buttonJoinGame;
    QPushButton *buttonRefreshGames;
    QTableView *tableView;
    QLabel *background;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainMenu)
    {
        if (MainMenu->objectName().isEmpty())
            MainMenu->setObjectName(QStringLiteral("MainMenu"));
        MainMenu->setEnabled(true);
        MainMenu->resize(1284, 807);
        centralWidget = new QWidget(MainMenu);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        buttonBack = new QPushButton(centralWidget);
        buttonBack->setObjectName(QStringLiteral("buttonBack"));
        buttonBack->setGeometry(QRect(20, 670, 131, 40));
        buttonJoinGame = new QPushButton(centralWidget);
        buttonJoinGame->setObjectName(QStringLiteral("buttonJoinGame"));
        buttonJoinGame->setGeometry(QRect(750, 590, 281, 61));
        buttonRefreshGames = new QPushButton(centralWidget);
        buttonRefreshGames->setObjectName(QStringLiteral("buttonRefreshGames"));
        buttonRefreshGames->setGeometry(QRect(260, 590, 281, 61));
        tableView = new QTableView(centralWidget);
        tableView->setObjectName(QStringLiteral("tableView"));
        tableView->setGeometry(QRect(150, 120, 991, 431));
        background = new QLabel(centralWidget);
        background->setObjectName(QStringLiteral("background"));
        background->setGeometry(QRect(0, 0, 1284, 760));
        background->setPixmap(QPixmap(QString::fromUtf8("Images/homeScreen.jpg")));
        background->setScaledContents(true);
        MainMenu->setCentralWidget(centralWidget);
        background->raise();
        tableView->raise();
        buttonBack->raise();
        buttonJoinGame->raise();
        buttonRefreshGames->raise();
        menuBar = new QMenuBar(MainMenu);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1284, 31));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainMenu->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainMenu);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainMenu->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());

        retranslateUi(MainMenu);

        QMetaObject::connectSlotsByName(MainMenu);
    } // setupUi

    void retranslateUi(QMainWindow *MainMenu)
    {
        MainMenu->setWindowTitle(QApplication::translate("MainMenu", "AllStarPoker", Q_NULLPTR));
        buttonBack->setText(QApplication::translate("MainMenu", "Back", Q_NULLPTR));
        buttonJoinGame->setText(QApplication::translate("MainMenu", "Join Game!", Q_NULLPTR));
        buttonRefreshGames->setText(QApplication::translate("MainMenu", "Refresh Games", Q_NULLPTR));
        background->setText(QString());
        menuFile->setTitle(QApplication::translate("MainMenu", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainMenu: public Ui_MainMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINMENU_H
