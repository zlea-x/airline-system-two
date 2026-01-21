#include "services/BookingSystem.h"
#include <QString>

// Constructor
BookingSystem::BookingSystem(AeroboxService* service)
    : aeroboxService(service),
      m_nextPassengerId(1),
      m_nextBookingId(1)
{
    seedSampleData();
}

// Accessors
const std::vector<Flight> BookingSystem::getFlights() const {
    return m_flights;
}

const std::vector<Booking> BookingSystem::getBookings() const {
    return m_bookings;
}

// Remote search by flight number + date
std::vector<Flight> BookingSystem::searchFlights(const std::string& flightNumber,
                                                 const std::string& date) const
{
    if (!aeroboxService) {
        // No remote service configured
        return {};
    }

    auto remote = aeroboxService->fetchFlights(
        QString::fromStdString(flightNumber),
        QString::fromStdString(date));

    return remote;
}

// Create booking for an existing flight
Booking BookingSystem::createBooking(int flightId, const std::string& passengerName)
{
    Flight flight = findFlightById(flightId);
    // Optional: check flight.getId() to detect invalid ID

    int pid = m_nextPassengerId++;
    m_passengers.emplace_back(pid, passengerName);

    int bid = m_nextBookingId++;
    m_bookings.emplace_back(bid, flightId, pid);
    return m_bookings.back();
}

// Find booking by ID (returns sentinel if not found)
Booking BookingSystem::findBookingById(int bookingId)
{
    for (auto& b : m_bookings) {
        if (b.getId() == bookingId) {
            return b;
        }
    }
    // Booking has no default ctor; return a sentinel
    return Booking(-1, -1, -1);
}

// Find flight by ID (returns sentinel if not found)
Flight BookingSystem::findFlightById(int flightId)
{
    for (auto& f : m_flights) {
        if (f.getId() == flightId) {
            return f;
        }
    }
    // Flight has no default ctor; return a sentinel
    return Flight(-1, "", "", "", "", FlightStatus::Scheduled);
}

// Assign seat using sentinels instead of pointers
bool BookingSystem::assignSeat(int bookingId, int row, int col)
{
    Booking booking = findBookingById(bookingId);
    if (booking.getId() == -1) {
        return false;
    }

    Flight flight = findFlightById(booking.getFlightId());
    if (flight.getId() == -1) {
        return false;
    }

    // seat map index is same as flight index
    int flightIndex = -1;
    for (int i = 0; i < static_cast<int>(m_flights.size()); ++i) {
        if (m_flights[i].getId() == flight.getId()) {
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

    booking.setSeat(row, col);
    booking.setStatus(BookingStatus::CheckedIn);
    return true;
}

// Update status of a local flight
void BookingSystem::updateFlightStatus(int flightId, FlightStatus status)
{
    for (auto& f : m_flights) {
        if (f.getId() == flightId) {
            f.setStatus(status);
            break;
        }
    }
}

// Cancel a local demo flight
bool BookingSystem::cancelFlight(int flightId)
{
    for (auto& f : m_flights) {
        if (f.getId() == flightId) {
            f.setStatus(FlightStatus::Cancelled);
            return true;
        }
    }
    return false;
}

// Demo data
void BookingSystem::seedSampleData()
{
    // Simple static flight list; times are just strings.
    m_flights.emplace_back(1, "XX",  "YY",  "2026-01-20 09:00", "2026-01-20 10:00");
    m_flights.emplace_back(2, "ABV", "KAN", "2026-01-20 11:00", "2026-01-20 12:00");
    m_flights.emplace_back(3, "LOS", "ACC", "2026-01-21 14:30", "2026-01-21 16:00");

    // One seat map per flight
    m_seatMaps.emplace_back(10, 4);
    m_seatMaps.emplace_back(10, 4);
    m_seatMaps.emplace_back(10, 4);
}
