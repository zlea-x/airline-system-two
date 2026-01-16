#include <QApplication>
#include <QMainWindow>
#include "services/BookingSystem.h"
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

    QMainWindow window;
    window.setWindowTitle("Flight Booking System");
    window.resize(800, 600);
    window.show();

    return app.exec();
}
