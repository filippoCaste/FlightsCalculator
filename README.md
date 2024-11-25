# Flight Booking System

## Description

This project is a Flight Booking System implemented in C++. It allows users to search for flight itineraries between two locations, taking into account flight times and fares. The system ensures that each flight arrives in time before the next flight and avoids cycles in the itinerary.

## Input

The input data consists of the number of flights followed by the details of each flight. Each flight detail includes:
- **Flight number**: A unique string of length 4 for each flight.
- **Airline**: A string of length 2 representing the airline.
- **Location 1**: A string of length 3 representing the departure location.
- **Location 2**: A string of length 3 representing the arrival location.
- **Departure time**: The departure time in HH:MM format.
- **Arrival time**: The arrival time in HH:MM format.
- **Fare**: The fare (price) as a floating-point number.

Example input:
```
4 TEST AF CDG NCE 10:00 12:15 210.89 
BLAH AF CDG EWR 17:00 23:59 700.10  
TAAA AF NCE CDG 14:00 16:30 200.35
TAA1 AF NCE EWR 09:00 15:00 600.98
```

## Main Functionalities

### 1. Search for Flight

Search if an itinerary from point A to point B (A != B) is possible. If a solution is found, it will be displayed; otherwise, a message indicating that it is not possible will be shown. The search stops at a maximum of 4 flights (`const int SOLUTION_MAX_FLIGHTS = 4`).

- **Flight combination**: Takes into account the time (each flight should arrive in time before the next flight).
- **No cycles**: Solutions should not contain cycles. We should be at each airport at most once (e.g., CDG-NCE and then NCE-MAD and then MAD-CDG is not permitted).

Solutions are displayed sorted by total price.

### Example NCE-EWR:
```
Solution #1:
TAA1 AF NCE EWR 09:00 15:00 600.98 

> Total price: 600.98

Solution #2:
TAAA AF NCE CDG 14:00 16:30 200.35 
BLAH AF CDG EWR 17:00 23:59 700.10 

> Total price: 900.45
```

## Compilation and Execution

To compile and run the program, use the following commands:
```sh
g++ -o flight_booking_system flight_booking_system.cpp
./flight_booking_system
```
