#ifndef BOOKFLIGHTDIALOG_H
#define BOOKFLIGHTDIALOG_H

#include <QDialog>
#include "services/BookingSystem.h"

class BookFlightDialog : public QDialog
{


public:
    explicit BookFlightDialog(int flightId, BookingSystem* system, QWidget* parent = nullptr);

private slots:
    void confirmBooking();

private:
    BookingSystem* m_system;
    int m_flightId;
};

#endif
