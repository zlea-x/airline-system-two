#include "ui/MainWindow.h"
#include "ui/BookFlightDialog.h"


#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QMessageBox>

MainWindow::MainWindow(BookingSystem* system, QWidget* parent)
    : QMainWindow(parent),
      m_system(system)
{
    setupUi();
    populateFlights(m_system->getFlights());
}

void MainWindow::setupUi()
{
    m_centralWidget = new QWidget(this);
    auto* mainLayout = new QVBoxLayout(m_centralWidget);

    // Search controls
    auto* searchLayout = new QHBoxLayout();
    m_originEdit = new QLineEdit(m_centralWidget);
    m_originEdit->setPlaceholderText("Origin (e.g. ABV)");
    m_destinationEdit = new QLineEdit(m_centralWidget);
    m_destinationEdit->setPlaceholderText("Destination (e.g. LOS)");
    m_searchButton = new QPushButton("Search", m_centralWidget);

    searchLayout->addWidget(m_originEdit);
    searchLayout->addWidget(m_destinationEdit);
    searchLayout->addWidget(m_searchButton);

    // Table of flights
    m_flightTable = new QTableWidget(m_centralWidget);
    m_flightTable->setColumnCount(5);
    QStringList headers;
    headers << "ID" << "Origin" << "Destination" << "Departure" << "Status";
    m_flightTable->setHorizontalHeaderLabels(headers);
    m_flightTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_flightTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_flightTable->setSelectionBehavior(QAbstractItemView::SelectRows);

    mainLayout->addLayout(searchLayout);
    mainLayout->addWidget(m_flightTable);

    // Check Flight Status (Online)

    auto* statusGroup = new QGroupBox("Check Flight Status (Online)", m_centralWidget);
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

    // Connect signals to plain member functions
    connect(m_searchButton, &QPushButton::clicked, this, [this]() {
        handleSearchClicked();
    });

    connect(m_flightTable, &QTableWidget::cellDoubleClicked,
            this, [this](int row, int column) {
        handleFlightDoubleClicked(row, column);
    });

    connect(m_checkStatusButton, &QPushButton::clicked, this, &MainWindow::onCheckStatusClicked); //check status button
}

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

void MainWindow::handleSearchClicked()
{
    std::string origin = m_originEdit->text().trimmed().toStdString();
    std::string destination = m_destinationEdit->text().trimmed().toStdString();

    auto results = m_system->searchFlights(origin, destination);
    populateFlights(results);
}

void MainWindow::handleFlightDoubleClicked(int row, int)
{
    if (row < 0 || row >= m_flightTable->rowCount()) {
        return;
    }

    bool ok = false;
    int flightId = m_flightTable->item(row, 0)->text().toInt(&ok);
    if (!ok) return;

    // Open booking dialog instead of showing details
    BookFlightDialog dialog(flightId, m_system, this);
    dialog.exec();
}

void MainWindow::onCheckStatusClicked() //check status 
{
    QString flightNumber = m_flightNumberEdit->text();
    QDate date = m_flightDateEdit->date();

    if (flightNumber.trimmed().isEmpty()) {
        m_statusValueLabel->setText("Enter a flight number.");
        return;
    }

    m_statusValueLabel->setText("Loading...");
    m_departureTimeValueLabel->clear();
    m_arrivalTimeValueLabel->clear();
    m_routeValueLabel->clear();

    //THE AEROBOX API CALL WILL GO HERE
}