# All Star Poker

## About

All Star Poker is a client/server application designed to host poker games. This project divides each component into it's own program and contains source code for each. In addition, each program follows the C++11 standard and uses QT 5 to provide GUI and Networking support. You can download QT from the official source [here](https://www.qt.io/download/). 

## Installation and Building

QT 5 is required to build the project. The offical source can be found [here](https://www.qt.io/download/). If you prefer to use QT Creator, you can simply open the .pro file using QT Creator and build from there. If on windows, I highly recommend using mingw as your compiler because it makes deployment easier. Instructions for deploying a QT application on windwos can be found [here](https://wiki.qt.io/Deploy_an_Application_on_Windows). On the other hand, if you'd like to build from the command line make sure qmake has been installed and use it to create your makefile. 

`$ cd your/project/dir
 $ qmake ASPServer.pro`

qmake will create the makefile, so the next step is to build the project using make. 

` $ make`

This will create the executable which can be called as such:

` $ ./ASPServer`

Note that the server component of All Star Poker has no GUI. It's specifically designed as a command line application so it can be hosted on a VPS and run in the background. 

