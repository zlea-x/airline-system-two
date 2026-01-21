#pragma once

#include "services/AeroboxService.h"
#include <QString>

class AeroboxServiceImpl : public AeroboxService {
public:
    AeroboxServiceImpl();

    std::vector<Flight> fetchFlights(const QString& flightNumber,
                                     const QString& date) override;

    bool cancelRemoteBooking(int bookingId) override;

private:
    QString m_apiKey;
};
