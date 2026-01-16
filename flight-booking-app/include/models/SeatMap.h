#ifndef SEATMAP_H
#define SEATMAP_H

#include <vector>

struct Seat {
    int row;
    int col;
    bool taken;
};

class SeatMap {
public:
    SeatMap(int rows = 10, int cols = 4);

    int rows() const;
    int cols() const;

    bool isTaken(int row, int col) const;
    bool takeSeat(int row, int col);

    const std::vector<std::vector<Seat>>& seats() const;

private:
    std::vector<std::vector<Seat>> m_seats;
};

#endif
