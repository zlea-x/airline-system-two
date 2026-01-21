#pragma once
#include <vector>
#include <QString>
#include "models/Flight.h"

class AeroboxService {
public:
    virtual ~AeroboxService() = default;

    // Fetch flights by flight number + date
    virtual std::vector<Flight> fetchFlights(
        const QString& flightNumber,
        const QString& date
    ) = 0;

    virtual bool cancelRemoteBooking(int bookingId) = 0;
};