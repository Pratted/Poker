#include <QCoreApplication>
#include <iostream>

#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Server server;

    if(!server.start()){
        std::cout << "Could not start server";
        a.exit();
    }
    if(!server.loadGames()){
        std::cout << "Could not load games.";
        a.exit();
    }

    std::cout << "Server started.\n";

    return a.exec();
}
