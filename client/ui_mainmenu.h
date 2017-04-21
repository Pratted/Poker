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
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainMenu
{
public:
    QWidget *centralWidget;
    QPushButton *buttonJoinGame;
    QLabel *background;
    QTableWidget *tableWidget;
    QMenuBar *menuBar;
    QMenu *menuFile;

    void setupUi(QMainWindow *MainMenu)
    {
        if (MainMenu->objectName().isEmpty())
            MainMenu->setObjectName(QStringLiteral("MainMenu"));
        MainMenu->setEnabled(true);
        MainMenu->resize(1284, 807);
        centralWidget = new QWidget(MainMenu);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        buttonJoinGame = new QPushButton(centralWidget);
        buttonJoinGame->setObjectName(QStringLiteral("buttonJoinGame"));
        buttonJoinGame->setGeometry(QRect(550, 450, 281, 61));
        background = new QLabel(centralWidget);
        background->setObjectName(QStringLiteral("background"));
        background->setGeometry(QRect(0, 0, 1284, 760));
        background->setPixmap(QPixmap(QString::fromUtf8("Images/homeScreen.jpg")));
        background->setScaledContents(true);
        tableWidget = new QTableWidget(centralWidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        tableWidget->setGeometry(QRect(100, 70, 1011, 341));
        MainMenu->setCentralWidget(centralWidget);
        background->raise();
        buttonJoinGame->raise();
        tableWidget->raise();
        menuBar = new QMenuBar(MainMenu);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1284, 17));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainMenu->setMenuBar(menuBar);

        menuBar->addAction(menuFile->menuAction());

        retranslateUi(MainMenu);

        QMetaObject::connectSlotsByName(MainMenu);
    } // setupUi

    void retranslateUi(QMainWindow *MainMenu)
    {
        MainMenu->setWindowTitle(QApplication::translate("MainMenu", "AllStarPoker", Q_NULLPTR));
        buttonJoinGame->setText(QApplication::translate("MainMenu", "Join Game!", Q_NULLPTR));
        background->setText(QString());
        menuFile->setTitle(QApplication::translate("MainMenu", "File", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainMenu: public Ui_MainMenu {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINMENU_H
