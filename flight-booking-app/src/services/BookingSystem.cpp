#include "services/BookingSystem.h"

BookingSystem::BookingSystem()
    : m_nextPassengerId(1),
      m_nextBookingId(1)
{
    seedSampleData(); // Abstraction: user of BookingSystem doesn't see this setup.
}

const std::vector<Flight>& BookingSystem::getFlights() const {
    return m_flights;
}

const std::vector<Booking>& BookingSystem::getBookings() const {
    return m_bookings;
}

std::vector<Flight> BookingSystem::searchFlights(const std::string& origin,
                                                 const std::string& destination) const
{
    std::vector<Flight> result;
    for (const auto& f : m_flights) {
        if ((origin.empty() || f.getOrigin() == origin) &&
            (destination.empty() || f.getDestination() == destination)) {
            result.push_back(f);
        }
    }
    return result;
}

Booking* BookingSystem::createBooking(int flightId, const std::string& passengerName)
{
    Flight* flight = findFlightById(flightId);
    if (!flight) {
        return nullptr;
    }

    int pid = m_nextPassengerId++;
    m_passengers.emplace_back(pid, passengerName);

    int bid = m_nextBookingId++;
    m_bookings.emplace_back(bid, flightId, pid);

    return &m_bookings.back();
}

Booking* BookingSystem::findBookingById(int bookingId)
{
    for (auto& b : m_bookings) {
        if (b.getId() == bookingId) {
            return &b;
        }
    }
    return nullptr;
}

Flight* BookingSystem::findFlightById(int flightId)
{
    for (auto& f : m_flights) {
        if (f.getId() == flightId) {
            return &f;
        }
    }
    return nullptr;
}

bool BookingSystem::assignSeat(int bookingId, int row, int col)
{
    Booking* booking = findBookingById(bookingId);
    if (!booking) {
        return false;
    }

    Flight* flight = findFlightById(booking->getFlightId());
    if (!flight) {
        return false;
    }

    // seat map index is same as flight index
    int flightIndex = -1;
    for (int i = 0; i < static_cast<int>(m_flights.size()); ++i) {
        if (m_flights[i].getId() == flight->getId()) {
            flightIndex = i;
            break;
        }
    }
    if (flightIndex < 0 || flightIndex >= static_cast<int>(m_seatMaps.size())) {
        return false;
    }

    SeatMap& map = m_seatMaps[flightIndex];
    if (!map.takeSeat(row, col)) {
        return false;
    }

    booking->setSeat(row, col);
    booking->setStatus(BookingStatus::CheckedIn);
    return true;
}

void BookingSystem::updateFlightStatus(int flightId, FlightStatus status)
{
    Flight* flight = findFlightById(flightId);
    if (flight) {
        flight->setStatus(status);
    }
}

void BookingSystem::seedSampleData()
{
    // Simple static flight list; times are just strings.
    m_flights.emplace_back(1, "ABV", "LOS", "2026-01-20 09:00", "2026-01-20 10:00");
    m_flights.emplace_back(2, "ABV", "KAN", "2026-01-20 11:00", "2026-01-20 12:00");
    m_flights.emplace_back(3, "LOS", "ACC", "2026-01-21 14:30", "2026-01-21 16:00");

    // One seat map per flight
    m_seatMaps.emplace_back(10, 4);
    m_seatMaps.emplace_back(10, 4);
    m_seatMaps.emplace_back(10, 4);
}
