# airline-system-two
# Flight Booking System

A C++/Qt desktop application for managing flight bookings and checking live flight status.  
The system combines a local demo booking engine with a real-time integration to the **AeroDataBox** API (via RapidAPI).

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Architecture](#architecture)
- [Technology Stack](#technology-stack)
- [Setup and Installation](#setup-and-installation)
- [Configuration (API Key)](#configuration-api-key)
- [How to Use](#how-to-use)
- [Design Decisions and Notes](#design-decisions-and-notes)
- [Possible Improvements](#possible-improvements)

---

## Overview

The Flight Booking System simulates a simple airline booking interface and demonstrates how an existing C++/Qt application can be extended with a third-party REST API.  
Users can browse sample flights, create bookings, cancel flights, and check live flight status by flight number and date using AeroDataBox. [file:26][file:28][file:27]

---

## Features

### Local booking features

- Display a table of **sample flights** with:
  - ID, origin, destination, departure time, and status. [file:28]
- Search local demo flights by **origin** and **destination** (top search bar). [file:26]
- Create a booking for a selected flight (demo dialog).
- Cancel a selected local flight and update its status to **Cancelled** in the table. [file:26][file:28]

### Online flight status (AeroDataBox)

- Enter **flight number** (e.g. `KL641`) and **date**. [file:26]
- The app calls the AeroDataBox **Flight by Number + Date** endpoint through RapidAPI:
  - `GET /flights/number/{flightNumber}/{dateLocal}?withAircraftImage=false&withLocation=false&withFlightPlan=false` [file:27][web:13]
- Displays:
  - Status (simplified text)
  - Departure time (local)
  - Arrival time (local)
  - Route (origin → destination) [file:27][file:26]

---

## Architecture

### Layers and main classes

- **UI Layer (Qt Widgets)**  
  - `MainWindow`
    - Shows the main table of flights and the “Check Flight Status (Online)” panel. [file:26]
    - Connects user actions (buttons, edits) to the `BookingSystem`.
  - `BookFlightDialog`
    - Simple dialog for confirming a booking for a selected flight. [file:25][file:33]

- **Domain / Service Layer**
  - `BookingSystem`
    - Central application service that manages:
      - `m_flights`, `m_bookings`, `m_passengers`, `m_seatMaps`. [file:28]
    - Provides operations:
      - `getFlights()`, `getBookings()`
      - `searchFlights(flightNumber, date)` → delegates to AeroDataBox. [file:28][file:35]
      - `createBooking`, `cancelFlight`, `assignSeat`, `updateFlightStatus`.
    - Seeds three sample flights and seat maps in `seedSampleData()`. [file:28]
  - `AeroboxService` / `AeroboxServiceImpl`
    - Interface + implementation for remote calls.
    - `AeroboxServiceImpl` uses `QNetworkAccessManager` to call AeroDataBox. [file:27][file:36][file:37]

- **Model Layer**
  - `Flight`: id, origin, destination, departure time, arrival time, status. [file:39][file:30]
  - `Booking`: id, flightId, passengerId, seat, status. [file:38][file:29]
  - `Passenger`: id, name. [file:41][file:32]
  - `SeatMap`: simple 2D seat occupancy representation. [file:40][file:31]

---

## Technology Stack

- **Language:** C++17 [file:43]
- **Framework:** Qt 6 (Core, Widgets, Network) [file:43]
- **Build System:** CMake (minimum 3.16) [file:43]
- **HTTP / JSON:** `QNetworkAccessManager`, `QNetworkRequest`, `QNetworkReply`, `QJsonDocument`, `QJsonObject`, `QJsonArray`. [file:27]
- **External API:** AeroDataBox via RapidAPI (`aerodatabox.p.rapidapi.com`). [file:27][web:13]

---

## Setup and Installation

### Prerequisites

- C++17‑compatible compiler (e.g. MSVC, Clang, or GCC). [file:43]
- Qt 6 with **Core**, **Widgets**, and **Network** modules installed. [file:43]
- CMake ≥ 3.16.
- A RapidAPI account with access to **AeroDataBox** (for `x-rapidapi-key`). [web:13]

### Building the project

From the project root:

```bash
mkdir build
cd build
cmake ..
cmake --build .
