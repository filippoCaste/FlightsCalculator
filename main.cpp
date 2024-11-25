#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cerrno>
#include <iomanip>

using namespace std;

int SOLUTION_MAX_FLIGHTS = 4;

class Flight {
    private:
        string flight_number, airline, location_1, location_2, departure_time, arrival_time;
        double fare;
        
    public:
        Flight(string flight_number, string airline, string location_1, string location_2, string departure_time, string arrival_time, double fare) {
            this->flight_number = flight_number;
            this->airline = airline;
            this->location_1 = location_1;
            this->location_2 = location_2;
            this->departure_time = departure_time;
            this->arrival_time = arrival_time;
            this->fare = fare;
        }
        
        bool operator==(const Flight& other) const {
            return this->flight_number.compare(other.get_flight_number()) == 0;
        }

        string get_flight_number() const{
            return this->flight_number;
        }

        string get_airline() const {
            return this->airline;
        }

        string get_location_1() const {
            return this->location_1;
        }

        string get_location_2() const {
            return this->location_2;
        }

        string get_departure_time() const {
            return this->departure_time;
        }

        string get_arrival_time() const {
            return this->arrival_time;
        }

        double get_fare() const {
            return this->fare;
        }

        // print the Flight info
        string print_flight() const {
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << this->fare;
            return this->flight_number + " " + this->airline + " " + this->location_1 + " " + this->location_2 + " " + this->departure_time + " " 
                + this->arrival_time + " € " + oss.str() + ";";
        }
};

class FlightsContainer {
    private:
        vector<Flight> flights;
    
    public:
        FlightsContainer() {
        }

        void add_flight(const Flight &f) {
            this->flights.push_back(f);
        }

        vector<Flight> get_flights() const {
            return this->flights;
        }

        // returns a string containing the flights details 
        string print_flightsContainer() const {
            string output;
            std::vector<Flight> sorted_flights = this->flights;
            
            for (const auto &f : sorted_flights) {
                output += f.print_flight() + "\n";
            }
            
            return output;
        }
        
        void erase(const Flight& f) {
            auto newEnd = remove(flights.begin(), flights.end(), f);
            flights.erase(newEnd, flights.end());
        }

        // returns the total sum of the fares
        double total_cost_flights() const {
            double sum = 0.0;
            for(auto &f : this->flights) {
                sum += f.get_fare();
            }
            return sum;
        }
        
        Flight get_flight_position(int pos) {
            return this->flights[pos];
        }

        // returns the list of locations where the itinerary goes through
        vector<string> list_passed_airports() const {
            vector<string> res;
            for(auto& f : this->flights) {
                res.push_back(f.get_location_2());
            }
            return res;
        }

        std::vector<Flight>::const_iterator begin() const {
            return flights.begin();
        }

        std::vector<Flight>::const_iterator end() const {
            return flights.end();
        }
};

class TourCalculator {

    private:
        FlightsContainer flights;

        FlightsContainer find_next_flights(string departure_location, string time_start) const {
            FlightsContainer next_flights;
            for(const auto& f : this->flights) {
                if(f.get_location_1().compare(departure_location) == 0 && f.get_departure_time() > time_start) {
                    next_flights.add_flight(f);
                }
            }
            return next_flights; 
        }

        FlightsContainer find_trip_recursive(string origin, string time_start, string destination, FlightsContainer temp_solution, int cnt, vector<FlightsContainer> &all_solutions) const {
            // exit conditions
            //// destination is reached in less than SOLUTION_MAX_FLIGHTS steps
            if(origin.compare(destination) == 0) {
                all_solutions.push_back(temp_solution);
            }

            //// if cnt is equal to the maximum nb of combinations without reaching the final destination, then stop and return empty FlightsContainer
            if(cnt == SOLUTION_MAX_FLIGHTS) {
                return FlightsContainer();
            }
        
            // recursive call
            for(auto f : find_next_flights(origin, time_start)) {
                vector<string> locations = temp_solution.list_passed_airports();
                
                // get the first Flight and add the departure point to the list (to avoid cycles also with the starting point)
                locations.push_back(temp_solution.get_flight_position(0).get_location_1());

                // if the destination of the flight f is not already visited, then add otherwise skip (to avoid cycles)
                if(find(locations.begin(), locations.end(), f.get_location_2()) == locations.end()) {
                    // add flight
                    temp_solution.add_flight(f);

                    // call recursion
                    find_trip_recursive(f.get_location_2(), f.get_arrival_time(), destination, temp_solution, cnt+1, all_solutions);

                    // backtracking
                    temp_solution.erase(f);
                } 
            }

            return FlightsContainer();
        }


    public:
        TourCalculator(FlightsContainer flights) {
            this->flights = flights;
        }

        vector<FlightsContainer> find_trip(string origin, string destination) const {
            vector<FlightsContainer> solution;
            
            // launch recursion every time there is a flight from the origin point
            for(Flight f : this->flights) {
                if(f.get_location_1().compare(origin) == 0) {
                    FlightsContainer temp_solution;
                    temp_solution.add_flight(f);
                    find_trip_recursive(f.get_location_2(), f.get_arrival_time(), destination, temp_solution, 1, solution);
                }
            }
            
            return solution;
        }

};

int main() {

    // import data
    ifstream f("flights.txt");

    if(!f.is_open()) {
        std::cout << "Error opening the file" << strerror(errno) << endl;
        return 1;
    }

    string line;
    FlightsContainer flights;

    while (getline(f, line)) {
        istringstream iss(line);
        string flight_number, airline, location_1, location_2, departure_time, arrival_time;
        double fare;

        if (!(iss >> flight_number >> airline >> location_1 >> location_2 >> departure_time >> arrival_time >> fare)) {
            std::cout << "Error parsing line: " << line << endl;
            continue;
        }

        flights.add_flight(Flight(flight_number, airline, location_1, location_2, departure_time, arrival_time, fare));
    }

    string starting_point;
    string destination;

    cout << "Enter starting point: ";
    cin >> starting_point;

    cout << "Enter destination: ";
    cin >> destination;
    
    std::transform(starting_point.begin(), starting_point.end(), starting_point.begin(), ::toupper);
    std::transform(destination.begin(), destination.end(), destination.begin(), ::toupper);

    TourCalculator tc(flights);

    int i=0;
    
    vector<FlightsContainer> solutions = tc.find_trip(starting_point, destination);
    std::sort(solutions.begin(), solutions.end(),
                  [](const FlightsContainer &a, const FlightsContainer &b) {
                      return a.total_cost_flights() < b.total_cost_flights();
        });
        
        
    if(solutions.size() == 0) {
        cout << "Trip from " << starting_point << " to " << destination << " NOT possible." << endl;
    } else {
        cout << "You can travel from " << starting_point << " to " << destination << " by choosing one of the following solutions:" << endl;
        for (const auto &res : solutions) {
            if(res.total_cost_flights() != .0) {
                std::cout << "\n# Solution " << to_string(i++) << endl;
                std::cout << res.print_flightsContainer() << endl;
                cout << "> Total cost: € " << res.total_cost_flights() << endl;
            }
        }
    }

    return 0;
}
