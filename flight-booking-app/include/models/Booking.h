#ifndef BOOKING_H
#define BOOKING_H

enum class BookingStatus {
    Booked,
    CheckedIn,
    Cancelled
};

class Booking {
public:
    Booking(int id, int flightId, int passengerId);

    int getId() const;
    int getFlightId() const;
    int getPassengerId() const;

    BookingStatus getStatus() const;
    void setStatus(BookingStatus status);

    int getSeatRow() const;
    int getSeatCol() const;
    void setSeat(int row, int col);

private:
    int m_id;
    int m_flightId;
    int m_passengerId;
    BookingStatus m_status;
    int m_seatRow;   // -1 if not assigned
    int m_seatCol;   // -1 if not assigned
};

#endif
