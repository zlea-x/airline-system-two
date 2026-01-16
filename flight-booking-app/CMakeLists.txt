cmake_minimum_required(VERSION 3.16)

project(FlightBookingApp LANGUAGES CXX)

# Find Qt 6 Widgets module
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Enable Qt's automatic MOC/UIC/RCC processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Define the executable and its source files
add_executable(FlightBookingApp
    app/main.cpp
    src/models/Flight.cpp
    src/models/Passenger.cpp
    src/models/Booking.cpp
    src/models/SeatMap.cpp
    src/services/BookingSystem.cpp
)

# Link against Qt Widgets
target_link_libraries(FlightBookingApp PRIVATE Qt6::Widgets)

# Where to look for headers
target_include_directories(FlightBookingApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
