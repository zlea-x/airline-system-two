#include <QApplication>
#include <QMainWindow>
#include "services/BookingSystem.h"
#include "ui/MainWindow.h"
#include <iostream>

int main(int argc, char *argv[])
{
    // Create the core booking engine (abstraction, encapsulation)
    BookingSystem system;

    // Quick console check: how many flights are loaded
    auto flights = system.getFlights();
    std::cout << "Flights loaded: " << flights.size() << std::endl;

    // Start Qt application (GUI layer)
    QApplication app(argc, argv);

    // Inheritance: MainWindow is a QMainWindow
    MainWindow window(&system);
    window.show();

    return app.exec();
}
