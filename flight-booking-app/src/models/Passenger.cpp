#include "models/Passenger.h"

Passenger::Passenger(int id, const std::string& name)
    : m_id(id), m_name(name)
{
}

int Passenger::getId() const {
    return m_id;
}

const std::string& Passenger::getName() const {
    return m_name;
}
