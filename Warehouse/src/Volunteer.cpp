

#include "../include/Volunteer.h"



////////////////////////////////////class Volunteer////////////////////////////////////////////

Volunteer::Volunteer(int id, const string &name) : completedOrderId(NO_ORDER),activeOrderId(NO_ORDER), id(id), name(name)  {}
int Volunteer:: getId() const { return id; }
const string &Volunteer::getName() const { return name; }
int Volunteer::getActiveOrderId() const { return activeOrderId; }
int Volunteer::getCompletedOrderId() const { return completedOrderId; }
bool Volunteer::isBusy() const {  return activeOrderId != NO_ORDER; }
void Volunteer::setCompletedOrderId(int orderId) {completedOrderId = orderId ;}



/////////////////////class CollectorVolunteer: public Volunteer//////////////////////////////

CollectorVolunteer::CollectorVolunteer(int id, const string &name, int coolDown) : Volunteer(id, name), coolDown(coolDown), timeLeft(0) {}

//virtual implemented
bool CollectorVolunteer::hasOrdersLeft() const { return true; }
bool CollectorVolunteer::canTakeOrder(const Order &order) const { return !isBusy(); }
void CollectorVolunteer:: acceptOrder(const Order &order) {
    activeOrderId = order.getId();
    timeLeft = coolDown;
}
void CollectorVolunteer::step() {
    decreaseCoolDown(); 
     if (timeLeft==0 && activeOrderId != NO_ORDER) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
    }
}
string CollectorVolunteer:: toString() const {
    if(isBusy()){
        return "VolunteerId: " + std::to_string(getId())+"\n"+
                "isBusy: True" + "\n"+
                "orderId: "+std::to_string(activeOrderId)+"\n"+
                "TimeLeft: "+ std::to_string(getTimeLeft())+"\n"+
                "OrdersLeft: No Limit";}
    else{
        return "VolunteerId: " + std::to_string(getId())+"\n"+
                "isBusy: False" + "\n"+
                "orderId: None"+"\n"+
                "TimeLeft: None"+"\n"+
                "OrdersLeft: No Limit";
                }
}
CollectorVolunteer *CollectorVolunteer::clone() const { return new CollectorVolunteer(*this); }
string CollectorVolunteer::getType() const {return "Collector";}

int CollectorVolunteer:: getCoolDown() const { return coolDown; }
int CollectorVolunteer:: getTimeLeft() const { return timeLeft; }
bool CollectorVolunteer:: decreaseCoolDown() {
    if (timeLeft > 0) {
        timeLeft--;
    }
    return timeLeft == 0;
}




/////////////////////class LimitedCollectorVolunteer: public CollectorVolunteer//////////////////////////////

LimitedCollectorVolunteer::LimitedCollectorVolunteer(int id, const string &name, int coolDown, int maxOrders) : CollectorVolunteer(id, name, coolDown), maxOrders(maxOrders), ordersLeft(maxOrders) {}

//virtual implemented
bool LimitedCollectorVolunteer::hasOrdersLeft() const 
{ 
    return (ordersLeft > 0); 
}
bool LimitedCollectorVolunteer::canTakeOrder(const Order &order) const 
{ 
    return !isBusy() && ordersLeft > 0; 
}
void LimitedCollectorVolunteer::acceptOrder(const Order &order)
{
    CollectorVolunteer::acceptOrder(order);
    ordersLeft--;
}

string LimitedCollectorVolunteer::toString() const
{
    if (isBusy())
    {
        return "VolunteerId: " + std::to_string(getId()) + "\n" +
               "isBusy: True" + "\n" +
               "orderId: " + std::to_string(activeOrderId) + "\n" +
               "TimeLeft: " + std::to_string(getTimeLeft()) + "\n" +
               "OrdersLeft: " + std::to_string(ordersLeft);
    }
    else
    {
        return "VolunteerId: " + std::to_string(getId()) + "\n" +
               "isBusy: False" + "\n" +
               "orderId: None" + "\n" +
               "TimeLeft: None" + "\n" +
               "OrdersLeft: " + std::to_string(ordersLeft);
    }
}

LimitedCollectorVolunteer *LimitedCollectorVolunteer::clone() const { 
    return new LimitedCollectorVolunteer(*this); }
string LimitedCollectorVolunteer::getType() const {return "LimitedCollector";}
int LimitedCollectorVolunteer::getMaxOrders() const
{
    return maxOrders;
}
int LimitedCollectorVolunteer::getNumOrdersLeft() const { return ordersLeft; }

int LimitedCollectorVolunteer::getOrdersLeft(){return ordersLeft;}

//////////////////////////class DriverVolunteer: public Volunteer/////////////////////////


DriverVolunteer::DriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep) : Volunteer(id, name), maxDistance(maxDistance), distancePerStep(distancePerStep), distanceLeft(0) {}

//virtual implemented

bool DriverVolunteer::hasOrdersLeft() const { return true; }
bool DriverVolunteer::canTakeOrder(const Order &order) const { return !isBusy() && order.getDistance() <= maxDistance; }
void DriverVolunteer::acceptOrder(const Order &order) { distanceLeft = order.getDistance(); activeOrderId = order.getId();}
void DriverVolunteer::step() { 
    decreaseDistanceLeft();
    if (getDistanceLeft()==0 && activeOrderId != NO_ORDER) {
        completedOrderId = activeOrderId;
        activeOrderId = NO_ORDER;
        }
}
string  DriverVolunteer::toString() const {
        if(isBusy()){
        return "VolunteerId: " + std::to_string(getId())+"\n"+
                "isBusy: True" + "\n"+
                "orderId: "+std::to_string(activeOrderId)+"\n"+
                "DistanceLeft: "+ std::to_string(getDistanceLeft())+"\n"+
                "OrdersLeft: No Limit";
                }
    else{
        return "VolunteerId: " + std::to_string(getId())+"\n"+
                "isBusy: False" + "\n"+
                "orderId: None"+"\n"+
                "DistanceLeft: None"+"\n"+
                "OrdersLeft: No Limit";
                }
}

DriverVolunteer *DriverVolunteer ::clone() const { return new DriverVolunteer(*this); }
string DriverVolunteer::getType() const {return "Driver" ;}
int DriverVolunteer::getDistanceLeft() const { return distanceLeft; }
int DriverVolunteer::getMaxDistance() const { return maxDistance; }
int DriverVolunteer::getDistancePerStep() const { return distancePerStep; }
bool DriverVolunteer::decreaseDistanceLeft() {
    if (distanceLeft > distancePerStep) {
        distanceLeft -= distancePerStep;
    }
    else
        distanceLeft=0;
    return distanceLeft <= 0;
}


void DriverVolunteer::setDistanceLeft(const Order &Order){
    distanceLeft= Order.getDistance();
}



/////////////////////class LimitedDriverVolunteer: public DriverVolunteer//////////////////////////////

LimitedDriverVolunteer::LimitedDriverVolunteer(int id, const string &name, int maxDistance, int distancePerStep,int maxOrders):DriverVolunteer(id,name,maxDistance,distancePerStep),maxOrders(maxOrders), ordersLeft(maxOrders){};

int LimitedDriverVolunteer::getMaxOrders() const {return maxOrders;};
int LimitedDriverVolunteer::getNumOrdersLeft() const {return ordersLeft;};

//virtual implemented
bool LimitedDriverVolunteer::hasOrdersLeft() const {return ordersLeft>0;};
bool LimitedDriverVolunteer::canTakeOrder(const Order &order) const  { return (!isBusy()&& hasOrdersLeft() && order.getDistance() <= getMaxDistance());}; // Signal if the volunteer is not busy, the order is within the maxDistance.
void LimitedDriverVolunteer::acceptOrder(const Order &order) {
    setDistanceLeft(order);// Assign distanceLeft to order's distance and decrease ordersLeft
    activeOrderId = order.getId();
    ordersLeft--;
} 
// void LimitedDriverVolunteer::step() { 
//     decreaseDistanceLeft();
//     if (getDistanceLeft()==0 && activeOrderId != NO_ORDER) {
//         completedOrderId = activeOrderId;
//         activeOrderId = NO_ORDER;
//         }
// }
string LimitedDriverVolunteer::toString() const {
    if(isBusy()){
        return "VolunteerId: " + std::to_string(getId())+"\n"+
                "isBusy: True" + "\n"+
                "orderId: "+std::to_string(activeOrderId)+"\n"+
                "DistanceLeft: "+ std::to_string(getDistanceLeft())+"\n"+
                "OrdersLeft: "+std::to_string(ordersLeft);}
    else{
        return "VolunteerId: " + std::to_string(getId())+"\n"+
                "isBusy: False" + "\n"+
                "orderId: None"+"\n"+
                "DistanceLeft: None"+"\n"+
                "OrdersLeft: "+std::to_string(ordersLeft);
                }
}
LimitedDriverVolunteer *LimitedDriverVolunteer::clone() const { return new LimitedDriverVolunteer(*this); };

string LimitedDriverVolunteer::getType() const {  return "LimitedDriver" ;}
int LimitedDriverVolunteer::getOrdersLeft(){return ordersLeft;}