
#include "../include/Action.h"



//////////////////////////////////Class BaseAction///////////////////////////////


BaseAction::BaseAction():errorMsg(),status(){}
ActionStatus BaseAction::getStatus() const {return status;}

void BaseAction::complete(){
    status=ActionStatus::COMPLETED;
}
void BaseAction::error(string errorMsg){
    status=ActionStatus::ERROR;
    errorMsg=errorMsg;
    cout<<errorMsg<<endl;
}
string BaseAction::getErrorMsg() const{
    return errorMsg;
}

string BaseAction::statToString(ActionStatus status) const {
    string s;
    if (status == ActionStatus::COMPLETED)
        s = "COMPLETED";
    else
        s = "ERROR";
    return s;
}

void BaseAction::setErrorMsg(string errorMsg){
    errorMsg = errorMsg;
}

///////////////////////////class SimulateStep : public BaseAction////////////////////////////


SimulateStep::SimulateStep(int numOfSteps) : numOfSteps(numOfSteps) {}

void SimulateStep::act(WareHouse &wareHouse) {
    for (int step = 0; step < numOfSteps; step++) {
            wareHouse.step();
        }
    complete();
    wareHouse.addAction(this);
}
        


std::string SimulateStep::toString() const {
    return "simulateStep" + std::to_string(numOfSteps);
}

SimulateStep *SimulateStep::clone() const {return new SimulateStep(*this);
}




/////////////////////////class AddOrder : public BaseAction//////////////////////////

AddOrder::AddOrder(int id): customerId(id){};

void AddOrder::act(WareHouse &wareHouse){
    if(customerId<wareHouse.getCustomerCounter()){         
        Customer &cust=wareHouse.getCustomer(customerId);
        if(cust.canMakeOrder()){
            wareHouse.increaseOrderCounter();
            int orderId = wareHouse.getOrderCounter();
            Order *newOrder =new Order(orderId ,customerId,cust.getCustomerDistance());
            (*newOrder).setStatus(OrderStatus::PENDING);
            (*newOrder).setDriverId(-1);
            (*newOrder).setCollectorId(-1);
            cust.addOrder(orderId);
            complete();
            wareHouse.addOrder(newOrder);
            }
        else{
            
            error("Cannot place this order");
        }
    wareHouse.addAction(this);
    }
    
    else{

        error("Cannot place this order"); 
        wareHouse.addAction(this);    
    }
}

string AddOrder::toString() const {  
    return "order " + std::to_string(customerId);
}

AddOrder *AddOrder::clone() const { return new AddOrder(*this);}



/////////////////////////class AddCustomer : public BaseAction/////////////////////////

AddCustomer::AddCustomer(const string &customerName, const string &customerType, int distance, int maxOrders):
    customerName(customerName) ,customerType((customerType=="civilian")?CustomerType::Civilian : CustomerType::Soldier),distance(distance),maxOrders(maxOrders){
       
    }

void AddCustomer::act(WareHouse &wareHouse) {
    int customerId = wareHouse.getCustomerCounter();
    if(customerType == CustomerType::Soldier){
        SoldierCustomer *newCustomer = new SoldierCustomer(customerId, customerName, distance, maxOrders);
        wareHouse.addCustomer(newCustomer);
    }
    else{
        CivilianCustomer *newCustomer = new CivilianCustomer(customerId, customerName, distance, maxOrders);
        wareHouse.addCustomer(newCustomer);
    }

    complete();
    wareHouse.addAction(this);
}

string AddCustomer::toString() const {            
    string type;
    if (customerType == CustomerType::Soldier)
        type = "soldier";
    else
        type = "civilian";

    string s = "customer " + customerName + " " + type + to_string(distance) + " "+  to_string(maxOrders);
    return s;}

AddCustomer *AddCustomer::clone() const  {return new AddCustomer(*this);}




/////////////////////////class PrintOrderStatus : public BaseAction/////////////////////////

PrintOrderStatus::PrintOrderStatus(int id):orderId(id){}
void PrintOrderStatus::act(WareHouse &wareHouse)  {
    if(orderId<=wareHouse.getOrderCounter()){
        Order ordToPrint = wareHouse.getOrder(orderId);
        string status =ordToPrint.ordStatToString(ordToPrint.getStatus());
        string custId=std::to_string(ordToPrint.getCustomerId());
        string collectorId=std::to_string(ordToPrint.getCollectorId());
        if (ordToPrint.getCollectorId()==-1)
            collectorId="None";
        string driverId=std::to_string(ordToPrint.getDriverId());
        if (ordToPrint.getDriverId()==-1)
            driverId="None";
        std::cout<<"OrderId: "+ to_string(orderId)<<std::endl;
        std::cout<<"OrderStatus: "+status <<std::endl;
        std::cout<<"CustomerID: "+ custId<<std::endl;
        std::cout<<"Collector: "+ collectorId<<std::endl;
        std::cout<<"Driver: "+ driverId<<std::endl;
        complete();
        wareHouse.addAction(this);
        }
    else{
        error("Order doesn't exist");
        wareHouse.addAction(this);
    }
    
}
string PrintOrderStatus::toString() const { return "orderStatus "+ std::to_string(orderId);}
PrintOrderStatus *PrintOrderStatus::clone() const {return new PrintOrderStatus(*this);}


/////////////////////////class PrintCustomerStatus: public BaseAction/////////////////////////

PrintCustomerStatus::PrintCustomerStatus(int customerId):customerId(customerId){setErrorMsg("Customer doesn't exist.");}
void PrintCustomerStatus::act(WareHouse &wareHouse){
    if(wareHouse.getCustomerCounter() < customerId){
        error("Customer doesn't exist.");
        wareHouse.addAction(this);
    }
    else{
        Customer& cust= wareHouse.getCustomer(customerId);
        std::cout<<"CustomerID: "+ to_string(customerId)<<std::endl;
        vector<int> orders=cust.getOrdersIds();
        for(int id: orders){
            std::cout<<"OrderID: "+ to_string(id)<<endl;
            Order order = wareHouse.getOrder(id);
            OrderStatus status = wareHouse.OrderStatusById(id);
            string s = order.ordStatToString(status);
            std::cout<<"OrderStatus: "+ s<<endl;  
            
        }
        std::cout<<"numOrdersLeft: "+ to_string((cust.getMaxOrders() - cust.getNumOrders()))<<endl;
        complete();
        wareHouse.addAction(this);
    }}

string PrintCustomerStatus::toString() const {
    return "customerStatus "+ to_string(customerId);}

PrintCustomerStatus *PrintCustomerStatus::clone() const {return new PrintCustomerStatus(*this);}





////////////////////////class PrintVolunteerStatus : public BaseAction////////////////////////

PrintVolunteerStatus::PrintVolunteerStatus(int id):volunteerId(id){}

void PrintVolunteerStatus::act(WareHouse &wareHouse) {
    if(wareHouse.isVolunteerExist(volunteerId)){
        Volunteer &vol = wareHouse.getVolunteer(volunteerId);
        cout<< vol.toString()<<endl;
        complete();
        wareHouse.addAction(this);
    }
    else {
        error("Volunteer doesn't exist.");
        wareHouse.addAction(this);
    }
}

string PrintVolunteerStatus::toString() const {return "volunteerStatus "+ to_string(volunteerId);}

PrintVolunteerStatus *PrintVolunteerStatus::clone() const { return new PrintVolunteerStatus(*this);}



/////////////////////////class PrintActionsLog : public BaseAction////////////////////

PrintActionsLog::PrintActionsLog(){}

void PrintActionsLog::act(WareHouse &wareHouse){
   // vector<BaseAction*> actionsLog = wareHouse.getActions();
    for ( const BaseAction* vecAction : wareHouse.getActions()) {
        string s=vecAction->toString();
       cout << (vecAction->toString()) << " " << statToString(vecAction->getStatus()) << endl;
    }
        complete();

    wareHouse.addAction(this);}

string PrintActionsLog::toString() const{return "log";}

PrintActionsLog *PrintActionsLog::clone() const {return new PrintActionsLog(*this);}



/////////////////////////class Close : public BaseAction////////////////////

Close::Close(){}

void Close::act(WareHouse &wareHouse) {
    for(Order* order:wareHouse.getPendingOrders())
            cout <<"OrderID: "+ to_string( order->getId())+", CustomerID: "+std::to_string(order->getCustomerId())+", Status: "+order->ordStatToString(order->getStatus()) << endl;
    for(Order* order:wareHouse.getInProcessOrders())
            cout <<"OrderID: "+ to_string( order->getId())+", CustomerID: "+std::to_string(order->getCustomerId())+", Status: "+order->ordStatToString(order->getStatus()) << endl;
    for(Order* order:wareHouse.getCompletedOrders())
           cout <<"OrderID: "+ to_string( order->getId())+", CustomerID: "+std::to_string(order->getCustomerId())+", Status: "+order->ordStatToString(order->getStatus()) << endl;
    complete();
    wareHouse.addAction(this);
    wareHouse.close();}

string Close::toString() const{return "Closed";}
Close *Close::clone() const {return new Close(*this);}


/////////////////////////class BackupWareHouse : public BaseAction////////////////////

BackupWareHouse::BackupWareHouse(){ }
void BackupWareHouse::act(WareHouse &wareHouse){
    if(backup==nullptr)
    backup =  new WareHouse(wareHouse);
    
    else{
        delete backup;
        backup =  new WareHouse(wareHouse);
    }
    complete();
    wareHouse.addAction(this);
}
string BackupWareHouse::toString() const {return "BackupWareHouse";}
BackupWareHouse *BackupWareHouse::clone() const {return new BackupWareHouse(*this);}


/////////////////////////class RestoreWareHouse : public BaseAction////////////////////

RestoreWareHouse::RestoreWareHouse(){setErrorMsg("No backup available");}
void RestoreWareHouse::act(WareHouse &wareHouse){
    if(backup == nullptr){
        error("No backup available");
        wareHouse.addAction(this);
    }
    else{
        wareHouse = *backup;
        complete(); 
        wareHouse.addAction(this);
        
    }
}
string RestoreWareHouse::toString() const {return "RestoreWareHouse";}
RestoreWareHouse *RestoreWareHouse::clone() const {return new RestoreWareHouse(*this);}

