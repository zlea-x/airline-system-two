#include "ui/MainWindow.h"
#include "ui/BookFlightDialog.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QDate>
#include <QDateEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QTableWidgetItem>

// Constructor
MainWindow::MainWindow(BookingSystem* system, QWidget* parent)
    : QMainWindow(parent),
      m_system(system)
{
    setupUi();
    populateFlights(m_system->getFlights());
}

// 6.1 setupUi: layout + connections
void MainWindow::setupUi()
{
    m_centralWidget = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(m_centralWidget);

    // Search controls (local demo: origin/destination)
    auto* searchLayout = new QHBoxLayout();
    m_originEdit = new QLineEdit(m_centralWidget);
    m_originEdit->setPlaceholderText("Origin (e.g. ABV)");
    m_destinationEdit = new QLineEdit(m_centralWidget);
    m_destinationEdit->setPlaceholderText("Destination (e.g. LOS)");
    m_searchButton = new QPushButton("Search", m_centralWidget);

    searchLayout->addWidget(m_originEdit);
    searchLayout->addWidget(m_destinationEdit);
    searchLayout->addWidget(m_searchButton);

    // Table of flights (local sample data)
    m_flightTable = new QTableWidget(m_centralWidget);
    m_flightTable->setColumnCount(5);
    QStringList headers;
    headers << "ID" << "Origin" << "Destination" << "Departure" << "Status";
    m_flightTable->setHorizontalHeaderLabels(headers);
    m_flightTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_flightTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_flightTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    // Cancel button (local)
    m_cancelFlightButton = new QPushButton("Cancel selected flight", m_centralWidget);

    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(m_flightTable);
    mainLayout->addWidget(m_cancelFlightButton);

    // Check Flight Status (Online)
    auto* statusGroupBox = new QGroupBox("Check Flight Status (Online)", m_centralWidget);
    auto* statusLayout = new QFormLayout(statusGroupBox);

    m_flightNumberEdit = new QLineEdit(statusGroupBox);
    statusLayout->addRow("Flight number:", m_flightNumberEdit);

    m_flightDateEdit = new QDateEdit(QDate::currentDate(), statusGroupBox);
    m_flightDateEdit->setCalendarPopup(true);
    statusLayout->addRow("Date:", m_flightDateEdit);

    m_checkStatusButton = new QPushButton("Check status", statusGroupBox);
    statusLayout->addRow(m_checkStatusButton);

    m_statusValueLabel = new QLabel(statusGroupBox);
    m_departureTimeValueLabel = new QLabel(statusGroupBox);
    m_arrivalTimeValueLabel = new QLabel(statusGroupBox);
    m_routeValueLabel = new QLabel(statusGroupBox);

    statusLayout->addRow("Status:", m_statusValueLabel);
    statusLayout->addRow("Departure time:", m_departureTimeValueLabel);
    statusLayout->addRow("Arrival time:", m_arrivalTimeValueLabel);
    statusLayout->addRow("Route:", m_routeValueLabel);

    mainLayout->addWidget(statusGroupBox);

    m_centralWidget->setLayout(mainLayout);
    setCentralWidget(m_centralWidget);

    setWindowTitle("Flight Booking System");
    resize(900, 600);

    // Connect signals
    connect(m_searchButton, &QPushButton::clicked,
            this, &MainWindow::handleSearchClicked);

    connect(m_flightTable, &QTableWidget::cellDoubleClicked,
            this, &MainWindow::handleFlightDoubleClicked);

    connect(m_checkStatusButton, &QPushButton::clicked,
            this, &MainWindow::onCheckStatusClicked);

    connect(m_cancelFlightButton, &QPushButton::clicked,
            this, &MainWindow::onCancelFlightClicked);
}

// 6.2 populateFlights: fill table from a list of Flight
void MainWindow::populateFlights(const std::vector<Flight>& flights)
{
    m_flightTable->setRowCount(static_cast<int>(flights.size()));

    for (int i = 0; i < static_cast<int>(flights.size()); ++i) {
        const Flight& f = flights[i];

        m_flightTable->setItem(i, 0,
            new QTableWidgetItem(QString::number(f.getId())));
        m_flightTable->setItem(i, 1,
            new QTableWidgetItem(QString::fromStdString(f.getOrigin())));
        m_flightTable->setItem(i, 2,
            new QTableWidgetItem(QString::fromStdString(f.getDestination())));
        m_flightTable->setItem(i, 3,
            new QTableWidgetItem(QString::fromStdString(f.getDepartureTime())));

        QString statusText = "Scheduled";
        switch (f.getStatus()) {
        case FlightStatus::Boarding:  statusText = "Boarding";  break;
        case FlightStatus::Departed:  statusText = "Departed";  break;
        case FlightStatus::Landed:    statusText = "Landed";    break;
        case FlightStatus::Cancelled: statusText = "Cancelled"; break;
        default: break;
        }

        m_flightTable->setItem(i, 4,
            new QTableWidgetItem(statusText));
    }
}

// 6.3 handleSearchClicked: local filter by origin/destination
void MainWindow::handleSearchClicked()
{
    std::string origin = m_originEdit->text().trimmed().toStdString();
    std::string destination = m_destinationEdit->text().trimmed().toStdString();

    std::vector<Flight> all = m_system->getFlights();
    std::vector<Flight> filtered;

    for (const auto& f : all) {
        bool matchOrigin = origin.empty() || f.getOrigin() == origin;
        bool matchDest   = destination.empty() || f.getDestination() == destination;

        if (matchOrigin && matchDest) {
            filtered.push_back(f);
        }
    }

    populateFlights(filtered);
}

// 6.4 handleFlightDoubleClicked: open booking dialog
void MainWindow::handleFlightDoubleClicked(int row, int)
{
    if (row < 0 || row >= m_flightTable->rowCount()) {
        return;
    }

    bool ok = false;
    int flightId = m_flightTable->item(row, 0)->text().toInt(&ok);
    if (!ok) return;

    BookFlightDialog dialog(flightId, m_system, this);
    dialog.exec();
}

// 6.5 onCheckStatusClicked: call remote API via BookingSystem
void MainWindow::onCheckStatusClicked()
{
    QString flightNumber = m_flightNumberEdit->text();
    QDate date = m_flightDateEdit->date();

    if (flightNumber.trimmed().isEmpty()) {
        m_statusValueLabel->setText("Enter a flight number.");
        return;
    }

    QString dateStr = date.toString("yyyy-MM-dd");

    m_statusValueLabel->setText("Loading...");
    m_departureTimeValueLabel->clear();
    m_arrivalTimeValueLabel->clear();
    m_routeValueLabel->clear();

    // Call BookingSystem -> Aerobox (search by flight number + date)
    auto results = m_system->searchFlights(
        flightNumber.toStdString(),
        dateStr.toStdString());

    if (results.empty()) {
        m_statusValueLabel->setText("No flights found.");
        return;
    }

    const Flight& f = results[0];

    m_statusValueLabel->setText("Scheduled");
    m_departureTimeValueLabel->setText(
        QString::fromStdString(f.getDepartureTime()));
    m_arrivalTimeValueLabel->setText(
        QString::fromStdString(f.getArrivalTime()));
    m_routeValueLabel->setText(
        QString("%1 → %2")
            .arg(QString::fromStdString(f.getOrigin()),
                 QString::fromStdString(f.getDestination())));
}

// 6.6 onCancelFlightClicked: local cancel
void MainWindow::onCancelFlightClicked()
{
    int row = m_flightTable->currentRow();
    if (row < 0 || row >= m_flightTable->rowCount()) {
        QMessageBox::warning(this, "Cancel flight", "Please select a flight first.");
        return;
    }

    bool ok = false;
    int flightId = m_flightTable->item(row, 0)->text().toInt(&ok);
    if (!ok) {
        QMessageBox::warning(this, "Cancel flight", "Invalid flight ID.");
        return;
    }

    auto reply = QMessageBox::question(
        this,
        "Cancel flight",
        QString("Are you sure you want to cancel flight %1?").arg(flightId));

    if (reply != QMessageBox::Yes) {
        return;
    }

    if (!m_system->cancelFlight(flightId)) {
        QMessageBox::warning(this, "Cancel flight", "Could not cancel this flight.");
        return;
    }

    populateFlights(m_system->getFlights());
    QMessageBox::information(this, "Cancel flight", "Flight cancelled successfully.");
}
