#ifndef FLIGHT_H
#define FLIGHT_H

#include <string>

enum class FlightStatus {
    Scheduled,
    Boarding,
    Departed,
    Landed,
    Cancelled
};

class Flight {
public:
    Flight(int id,
           const std::string& origin,
           const std::string& destination,
           const std::string& departureTime,
           const std::string& arrivalTime,
           FlightStatus status = FlightStatus::Scheduled);

    int getId() const;
    const std::string& getOrigin() const;
    const std::string& getDestination() const;
    const std::string& getDepartureTime() const;
    const std::string& getArrivalTime() const;
    FlightStatus getStatus() const;

    void setStatus(FlightStatus status);

private:
    int m_id;
    std::string m_origin;
    std::string m_destination;
    std::string m_departureTime;
    std::string m_arrivalTime;
    FlightStatus m_status;
};

#endif
