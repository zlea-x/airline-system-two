#include "ui/BookFlightDialog.h" 
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>

BookFlightDialog::BookFlightDialog(int flightId, BookingSystem* system, QWidget* parent)
    : QDialog(parent), m_system(system), m_flightId(flightId)
{
    setWindowTitle("Book Flight");
    resize(400, 250);
    
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel(QString("Flight ID: %1").arg(m_flightId)));
    
    layout->addWidget(new QLabel("Passenger Name:"));
    auto* nameEdit = new QLineEdit();
    nameEdit->setPlaceholderText("Enter full name");
    layout->addWidget(nameEdit);
    
    auto* btnLayout = new QHBoxLayout();
    auto* bookBtn = new QPushButton("Book Flight");
    auto* cancelBtn = new QPushButton("Cancel");
    btnLayout->addWidget(bookBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);
    
    connect(bookBtn, &QPushButton::clicked, this, &BookFlightDialog::confirmBooking);
    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
}

void BookFlightDialog::confirmBooking()
{
    QMessageBox::information(this, "Success", "Booking confirmed!");
    accept();
}
