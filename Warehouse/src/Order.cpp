
#include  "../include/Order.h"



//////////////////////////////////////////Class Order//////////////////////////////


Order::Order(int id, int customerId, int distance) : id(id), customerId(customerId), distance(distance), status(OrderStatus::PENDING), collectorId(NO_VOLUNTEER), driverId(NO_VOLUNTEER) {}

int Order::getId() const { return id; }

int Order::getCustomerId() const { return customerId; }

void Order::setStatus(OrderStatus status) { this->status = status; }

void Order::setCollectorId(int collectorId) { this->collectorId = collectorId; }

void Order::setDriverId(int driverId) { this->driverId = driverId; }

int Order::getCollectorId() const { return collectorId; }

int Order::getDriverId() const { return driverId; }

OrderStatus Order:: getStatus() const { return status; }

const string Order:: toString() const {
    return "OrderID: "+to_string(id)+", Order Status: "+", Costumer Id: "+ to_string(customerId)+", Collector: "+ to_string(collectorId)+"Driver Id: "+ to_string(driverId)+", distance: "+to_string(distance);
          
}

int Order::getDistance() const { return distance ; }

string Order::ordStatToString(OrderStatus status) {
    string s;
    if (status == OrderStatus::PENDING)
        s = "PENDING";
    else if(status == OrderStatus::COLLECTING)
        s = "COLLECTING";
    else if(status == OrderStatus::DELIVERING)
        s = "DELIVERING";
    else
        s = "COMPLETED";
    return s;
};

Order *Order::clone() const { return new Order(*this); }