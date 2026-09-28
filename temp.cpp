#include <iostream>
#include <bits/stdc++.h>
using namespace std;

enum class vehicletype{ /*makes code more readable and maintainable. 
    access them as VehicleType::car so that there
    are no naming conflicts with other identifiers.
    no implicit conversion */
    car,
    bike
};

// vehicles 
class vehicle{
    private:
    vehicletype type;
    int numplate;

    public:
    vehicle(vehicletype t, int n): type(t), numplate(n) {};

    vehicletype getVehicleType() const {
        return type;
    }

    int getNumplate() const {
        return numplate;
    }

    virtual void display() = 0;   /* one pure virtual function is enough to make the class abstract.
     An object of an abstract class cannot be created. 
    It can only be used as a base class for other classes*/

    virtual ~vehicle() = default; /*virtual destructor to ensure proper cleanup of derived class objects in case of 
    runtime polymorphism */
};

class car: public vehicle{

    public:
    car(int n): vehicle(vehicletype::car, n) {}; // hardcore so bike type cannot be passed to car constructor

    void display() override {
        cout << "Car with number plate: " << getNumplate() << endl;
    }
};


class bike: public vehicle{

    public:
    bike(int n): vehicle(vehicletype::bike, n) {};// same logic as car constructor

    void display() override {
        cout << "Bike with number plate: " << getNumplate() << endl;
    }
};



class ticket{
    private:
    static int ticketnumber;

    public:
     ticket() {
        ticketnumber = ticketnumber+1;
    }
    void displayTicket() {
        cout << "Ticket issued with number: " << ticketnumber << endl;
    }
};
int ticket::ticketnumber = 0; // static member variable initialization


class spot{
    private:
    vehicle* v; 
    bool occupied;
    ticket* t; // pointer to ticket object

    public:
    spot(): v(nullptr), occupied(false) {};

    bool occupiedStatus() const {
        return occupied;
    }

    bool parkVehicle(vehicle* vehicle) {
        if (!occupied) {
            v = vehicle;
            occupied = true;
            t = new ticket();
            cout << "Vehicle parked." << endl;
            t->displayTicket();
            return true;
        } 
            cout << "Spot is already occupied." << endl;
            return false;
    }

    void removeVehicle() {
        if (occupied) {
            delete v; // free the memory allocated for the vehicle
            v = nullptr;
            occupied = false;
            cout << "Vehicle removed." << endl;
            delete t; // free the memory allocated for the ticket
            t = nullptr;
        } else {
            cout << "Spot is already empty." << endl;
        }
    }

    vehicle* getVehicle() const {
        return v;
    }

};


class parkingfloor{
    private:
    vector <spot*> spots;
    public:
    parkingfloor(int numSpots) {
        for (int i = 0; i < numSpots; ++i) {
            spots.push_back(new spot());
        }
    }

    bool parkVehicle(vehicle* v) {
        for (spot* s : spots) {
            if (!s->occupiedStatus()) {
                s->parkVehicle(v);
                return true;
            }
        }
        return false; // No available spot on this floor
    }

    bool removeVehicle(vehicle* v) {
        for (spot* s : spots) {
            if (s->getVehicle() == v) {
                s->removeVehicle();
                return true;
            }
        }
        return false; // Vehicle not found on this floor
    }
};


class parkinglot {
private:
    vector<parkingfloor* > floors;
public:
    parkinglot(int numFloors, int spotsPerFloor) {
        for (int i = 0; i < numFloors; i++) {
            floors.emplace_back(new parkingfloor(spotsPerFloor));
        }
    }

    void parkVehicle(vehicle* v) {
        for (parkingfloor* f : floors) {
            if (f->parkVehicle(v)) {
                return;
            }
        }
        cout << "Parking lot is full." << endl;
    }


    void removeVehicle(vehicle* v) {
        for (parkingfloor* f : floors) {
            if (f->removeVehicle(v)) {
                return;
            }
        }
        cout << "Vehicle not found in the parking lot." << endl;
    }
};


int main() {
    parkinglot pl(2, 3); 
    vehicle* car1 = new car(123);
    vehicle* bike1 = new bike(456);
    pl.parkVehicle(car1);
    pl.parkVehicle(bike1);
    pl.removeVehicle(car1);
    pl.removeVehicle(bike1);
    return 0;
}