#include "models/Flight.h"

Flight::Flight(int id,
               const std::string& origin,
               const std::string& destination,
               const std::string& departureTime,
               const std::string& arrivalTime,
               FlightStatus status)
    : m_id(id),
      m_origin(origin),
      m_destination(destination),
      m_departureTime(departureTime),
      m_arrivalTime(arrivalTime),
      m_status(status)
{
}

int Flight::getId() const {
    return m_id;
}

const std::string& Flight::getOrigin() const {
    return m_origin;
}

const std::string& Flight::getDestination() const {
    return m_destination;
}

const std::string& Flight::getDepartureTime() const {
    return m_departureTime;
}

const std::string& Flight::getArrivalTime() const {
    return m_arrivalTime;
}

FlightStatus Flight::getStatus() const {
    return m_status;
}

void Flight::setStatus(FlightStatus status) {
    m_status = status;
}
