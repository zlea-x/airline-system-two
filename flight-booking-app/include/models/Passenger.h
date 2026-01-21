#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>

class Passenger {
public:
    Passenger() = default;
    Passenger(int id, const std::string& name);

    int getId() const;
    const std::string& getName() const;

private:
    int m_id{};
    std::string m_name;
};

#endif
