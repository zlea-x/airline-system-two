#include "models/SeatMap.h"

SeatMap::SeatMap(int rows, int cols)
{
    m_seats.resize(rows);
    for (int r = 0; r < rows; ++r) {
        m_seats[r].resize(cols);
        for (int c = 0; c < cols; ++c) {
            m_seats[r][c] = Seat{r, c, false};
        }
    }
}

int SeatMap::rows() const {
    return static_cast<int>(m_seats.size());
}

int SeatMap::cols() const {
    return m_seats.empty() ? 0 : static_cast<int>(m_seats[0].size());
}

bool SeatMap::isTaken(int row, int col) const {
    if (row < 0 || col < 0 || row >= rows() || col >= cols())
        return true;
    return m_seats[row][col].taken;
}

bool SeatMap::takeSeat(int row, int col) {
    if (row < 0 || col < 0 || row >= rows() || col >= cols())
        return false;
    if (m_seats[row][col].taken)
        return false;
    m_seats[row][col].taken = true;
    return true;
}

const std::vector<std::vector<Seat>>& SeatMap::seats() const {
    return m_seats;
}
