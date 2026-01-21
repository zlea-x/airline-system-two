#include <QApplication>

#include "services/BookingSystem.h"
#include "services/AeroboxServiceImpl.h"
#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);   

    AeroboxServiceImpl aeroboxService;
    BookingSystem system(&aeroboxService);

    MainWindow window(&system);
    window.show();

    return app.exec();
}