#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDateEdit>
#include <QLabel>
#include "services/BookingSystem.h"

// Inheritance: MainWindow IS-A QMainWindow
class MainWindow : public QMainWindow
{ 
    
public:
    explicit MainWindow(BookingSystem* system, QWidget* parent = nullptr);

private:
    BookingSystem* m_system;   // Abstraction: talks to BookingSystem

    // Encapsulation: UI widgets are private members
    QWidget*      m_centralWidget;
    QLineEdit*    m_originEdit;
    QLineEdit*    m_destinationEdit;
    QPushButton*  m_searchButton;
    QTableWidget* m_flightTable;

    // Widgets for Check Flight Status 
    QPushButton* m_cancelFlightButton;

    QLineEdit* m_flightNumberEdit;
    QDateEdit* m_flightDateEdit;
    QPushButton* m_checkStatusButton;  
    QLabel* m_statusValueLabel;
    QLabel* m_departureTimeValueLabel;
    QLabel* m_arrivalTimeValueLabel;
    QLabel* m_routeValueLabel;

    void setupUi();
    void populateFlights(const std::vector<Flight>& flights);

    void handleSearchClicked();
    void handleFlightDoubleClicked(int row, int column);


private slots:
    void onCheckStatusClicked(); 
    void onCancelFlightClicked();
};

#endif
