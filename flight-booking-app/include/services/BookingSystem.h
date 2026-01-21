#ifndef BOOKINGSYSTEM_H
#define BOOKINGSYSTEM_H

#include <vector>
#include <string>
#include "models/Flight.h"
#include "models/Passenger.h"
#include "models/Booking.h"
#include "models/SeatMap.h"
#include "services/AeroboxService.h"

class BookingSystem {
public:
    explicit BookingSystem(AeroboxService* service = nullptr);

    // Accessors
    const std::vector<Flight> getFlights() const;
    const std::vector<Booking> getBookings() const;

    // Use cases

    // Remote search: flight number + date (used for online status check)
    std::vector<Flight> searchFlights(const std::string& flightNumber,
                                      const std::string& date) const;

    Booking createBooking(int flightId, const std::string& passengerName);
    Booking findBookingById(int bookingId);
    Flight findFlightById(int flightId);

    bool assignSeat(int bookingId, int row, int col);
    void updateFlightStatus(int flightId, FlightStatus status);

    // Cancel a flight by ID (local demo flights)
    bool cancelFlight(int flightId);

private:
    AeroboxService* aeroboxService = nullptr;

    std::vector<Flight> m_flights;
    std::vector<Passenger> m_passengers;
    std::vector<Booking> m_bookings;
    std::vector<SeatMap> m_seatMaps; // one per flight

    int m_nextPassengerId;
    int m_nextBookingId;

    void seedSampleData(); // creates some demo flights
};

#endif
