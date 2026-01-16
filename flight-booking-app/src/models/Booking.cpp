#include "models/Booking.h"

Booking::Booking(int id, int flightId, int passengerId)
    : m_id(id),
      m_flightId(flightId),
      m_passengerId(passengerId),
      m_status(BookingStatus::Booked),
      m_seatRow(-1),
      m_seatCol(-1)
{
}

int Booking::getId() const {
    return m_id;
}

int Booking::getFlightId() const {
    return m_flightId;
}

int Booking::getPassengerId() const {
    return m_passengerId;
}

BookingStatus Booking::getStatus() const {
    return m_status;
}

void Booking::setStatus(BookingStatus status) {
    m_status = status;
}

int Booking::getSeatRow() const {
    return m_seatRow;
}

int Booking::getSeatCol() const {
    return m_seatCol;
}

void Booking::setSeat(int row, int col) {
    m_seatRow = row;
    m_seatCol = col;
}
