#include "../include/WareHouse.h"


//rule of 5
//constructor
WareHouse::WareHouse(const string &configFilePath) : isOpen(false), actionsLog(),volunteers(), pendingOrders(),inProcessOrders()
        ,completedOrders(), customers(), customerCounter(0), volunteerCounter(0), orderCounter(-1) {
        parsingTxt(configFilePath);
    }

//destructor
 WareHouse::~WareHouse() {
        // Clean up dynamically allocated memory (e.g., customers, volunteers, orders)
        for (Customer *customer : customers) {
            delete customer;
        }
        
        for (Volunteer *volunteer : volunteers) {
            delete volunteer;
        }

        for (Order *order : pendingOrders) {
            delete order;
        }
    
        for (Order *order : inProcessOrders) {
            delete order;
        }
    
        for (Order *order : completedOrders) {
            delete order;
        }

        // Clean up actionsLog
        for (BaseAction *action : actionsLog) {
            delete action;
        }
    
    }

//Copy Constructor
WareHouse::WareHouse(const WareHouse& other):isOpen(other.isOpen), actionsLog(),volunteers(), pendingOrders(),inProcessOrders()
                ,completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter){
    for ( Customer* customer : other.customers) {
            customers.push_back(customer->clone()); // Copy each object
        }

    for ( Order* order : other.pendingOrders) {
            pendingOrders.push_back(order->clone()); // Copy each object
        }
    for ( Order* order : other.inProcessOrders) {
            inProcessOrders.push_back(order->clone()); // Copy each object
        }
    for ( Order* order : other.completedOrders) {
            completedOrders.push_back(order->clone()); // Copy each object
        }
    for ( Volunteer* volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone()); // Copy each object
        }
    for ( BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone()); // Copy each object
        }
    
}

//Copy Assignment Operator
WareHouse& WareHouse:: operator=(const WareHouse& other){
    if(this!=&other){
    for ( Customer* customer : customers) {
            delete customer; 
        }

    for ( Order* order : pendingOrders) {
            delete order;
        }
    for ( Order* order : inProcessOrders) {
            delete order;
        }
    for ( Order* order : completedOrders) {
            delete order;
        }
    for ( Volunteer* volunteer : volunteers) {
            delete volunteer; 
        }
    for ( BaseAction* action : actionsLog) {
            delete action; 
        }
    
    customers.clear();
    volunteers.clear();
    pendingOrders.clear();
    inProcessOrders.clear();
    completedOrders.clear();
    actionsLog.clear();

    for ( Customer* customer : other.customers) {
            customers.push_back(customer->clone()); // Copy each object
        }

    for ( Order* order : other.pendingOrders) {
            pendingOrders.push_back(order->clone()); // Copy each object
        }
    for ( Order* order : other.inProcessOrders) {
            inProcessOrders.push_back(order->clone()); // Copy each object
        }
    for ( Order* order : other.completedOrders) {
            completedOrders.push_back(order->clone()); // Copy each object
        }
    for ( Volunteer* volunteer : other.volunteers) {
            volunteers.push_back(volunteer->clone()); // Copy each object
        }
    for ( BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action->clone()); // Copy each object
        }
    isOpen=other.isOpen;
    customerCounter=other.customerCounter;
    volunteerCounter=other.volunteerCounter;
    orderCounter=other.orderCounter;
    }
     return *this;

}

//move constructor
WareHouse::WareHouse(WareHouse&& other):isOpen(other.isOpen), actionsLog(),volunteers(), pendingOrders(),inProcessOrders()
                ,completedOrders(), customers(), customerCounter(other.customerCounter), volunteerCounter(other.volunteerCounter), orderCounter(other.orderCounter){
    for ( Customer* customer : other.customers) {
            customers.push_back(customer);
            customer=nullptr; 
        }
    other.customers.clear();
    for ( Order* order : other.pendingOrders) {
            pendingOrders.push_back(order); 
            order=nullptr;
        }
    other.pendingOrders.clear();
    for ( Order* order : other.inProcessOrders) {
            inProcessOrders.push_back(order); 
            order=nullptr;
        }
    other.inProcessOrders.clear();
    for ( Order* order : other.completedOrders) {
            completedOrders.push_back(order); 
            order=nullptr;
        }
    other.completedOrders.clear();
    for ( Volunteer* volunteer : other.volunteers) {
            volunteers.push_back(volunteer); 
            volunteer=nullptr;
        }
    other.volunteers.clear();
    for ( BaseAction* action : other.actionsLog) {
            actionsLog.push_back(action);
            action=nullptr;
        }
    other.actionsLog.clear();

}

//move assignment operator
WareHouse& WareHouse::operator=(WareHouse &&other){
    if (this!=&other){
        isOpen=other.isOpen;
        customerCounter=other.customerCounter;
        volunteerCounter=other.volunteerCounter;
        orderCounter=other.orderCounter;
        for ( BaseAction* action : actionsLog) 
            delete action;
        for ( BaseAction* action : other.actionsLog){
            actionsLog.push_back(action);
            action=nullptr; 
        }
        
        for ( Volunteer* volunteer : volunteers) 
            delete volunteer;
        for ( Volunteer* volunteer : other.volunteers){
            volunteers.push_back(volunteer);
            volunteer=nullptr; 
        }

        for ( Order* order : completedOrders) 
            delete order;
        for ( Order* order : other.completedOrders){
            completedOrders.push_back(order);
            order=nullptr; 
        }

        for ( Order* order : inProcessOrders) 
            delete order;
        for ( Order* order : other.inProcessOrders){
            inProcessOrders.push_back(order);
            order=nullptr; 
        }

        for ( Order* order : pendingOrders) 
            delete order;
        for ( Order* order : other.pendingOrders){
            pendingOrders.push_back(order);
            order=nullptr; 
        }

        for ( Customer* customer : customers) 
            delete customer;
        for ( Customer* customer : other.customers){
            customers.push_back(customer);
            customer=nullptr; 
        }
        
}
return *this; 
}

// void WareHouse::parsingTxt(const string &configFilePath) {

void WareHouse::parsingTxt(const string&configFilePath){
	ifstream configFile(configFilePath);

	if (!configFile.is_open()) {
		cerr << "Error: Unable to open the configuration file." << endl;
		// Handle the error accordingly
		return;
	}

	string line;
	while (getline(configFile, line))
	{
		// Ignore lines starting with '#'
		if (line.empty() || line[0] == '#')
			continue;


		// Remove comments at the end of the line
		line = line.substr(0, line.find('#'));

		istringstream iss(line);
		string type;
		iss >> type;

		if (type == "customer")
		{
			// # Customers - customer <customer_name> <customer_type> <customer_distance> <max_orders>
			string name, customerType;
			int distance, maxOrders;
			iss >> name >> customerType >> distance >> maxOrders;
			// Create Customer object and add it to your Warehouse
			if (customerType == "civilian"){
				customers.push_back(new CivilianCustomer(customerCounter, name, distance, maxOrders));
                customerCounter++;
            }
			else{
				customers.push_back(new SoldierCustomer(customerCounter, name, distance, maxOrders));
                customerCounter++;
            }
		}

		else if (type == "volunteer")
		{

			//# Volunteers - volunteer <volunteer_name> <volunteer_role> <volunteer_coolDown>/<volunteer_maxDistance> <distance_per_step>(for drivers only) <volunteer_maxOrders>(optional)
			string name, role;
			int cooldown, maxDistance, distancePerStep, maxOrders;
			iss >> name >> role;


			if (role == "collector" || role == "limited_collector")
			{
				iss >> cooldown >> maxOrders;

				if (iss.fail()) {// no maxOrders has been given
					volunteers.push_back(new CollectorVolunteer(volunteerCounter, name, cooldown));
                    volunteerCounter++;
                }
				else{
					volunteers.push_back(new LimitedCollectorVolunteer(volunteerCounter, name, cooldown, maxOrders));
                    volunteerCounter++;
                    }
			}

			else if (role == "driver" || role == "limited_driver")
			{
				iss >> maxDistance >> distancePerStep >> maxOrders;

				if (iss.fail()) {// no maxOrders has been given
					volunteers.push_back(new DriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep));
                    volunteerCounter++;
                    }
				else{
					volunteers.push_back(new LimitedDriverVolunteer(volunteerCounter, name, maxDistance, distancePerStep, maxOrders));
                    volunteerCounter++;}
			}
		}
		else
			continue; // ERROR
	}

	configFile.close();
}


void WareHouse::start() {
    open();
    cout << "Warehouse is open!" << endl;
   //handleInputAction2();
    
    string userInput;
    while (isOpen) {
        cout << "Enter a command: ";
        std::getline(std::cin, userInput);

        // Use stringstream to extract words

        std::istringstream iss(userInput);
        
        std::string firstWord, secondWord, thirdWord;
        
        string first, second;
        iss >> firstWord;

        if (firstWord == "order") {
            iss >> first; 
            AddOrder *order = new AddOrder(stoi(first));
            order->act(*this);
        } 
        else if (firstWord == "orderStatus") {
            iss >> first; 
            PrintOrderStatus *action = new PrintOrderStatus(stoi(first));
            action->act(*this);
        } 
        else if (firstWord == "volunteerStatus") {
            iss >> first; 
            PrintVolunteerStatus *action = new PrintVolunteerStatus(stoi(first));
            action->act(*this);
        } 
        else if (firstWord == "step") {
            iss >> first; 
            SimulateStep *action = new SimulateStep(stoi(first));
            action->act(*this);
        } 
        else if (firstWord == "customerStatus") {
            iss >> first; 
            PrintCustomerStatus *action = new PrintCustomerStatus(stoi(first));
            action->act(*this);
        }
         else if (firstWord == "customer") {
            iss >> secondWord >>thirdWord; 
            iss >> first >> second; 
			AddCustomer *action = new AddCustomer(secondWord, thirdWord, stoi(first), stoi(second));
            action->act(*this);
	
         } 
        else if (firstWord == "log") {
            PrintActionsLog *action = new PrintActionsLog();
            action->act(*this);
        } 
        else if (firstWord == "backup") {
            BackupWareHouse *action = new BackupWareHouse();
            action->act(*this);
        } 
        else if (firstWord == "restore") {
            RestoreWareHouse *action = new RestoreWareHouse();
            action->act(*this);
        } 

        else if (firstWord == "close") {
            Close *action = new Close();
            action->act(*this);
        } 
        else {
            cout << "No such command. Enter your command again" << endl;
        }
        
    }
}


void WareHouse::step(){

    // Stage 1: Assign orders to volunteers
    for (int i=0; i<(int)pendingOrders.size();) {
        bool stayInPlace = false;
        //if need to assign to a collector
        Order *currOrder = pendingOrders[i];
        if(currOrder->getStatus()== OrderStatus::PENDING){
            for (int j=0; j<(int)volunteers.size();j++){
                Volunteer *currVolunteer = volunteers[j];
                if((currVolunteer->getType()=="Collector" && currVolunteer->canTakeOrder(*currOrder))|| (currVolunteer->getType()=="LimitedCollector" &&currVolunteer->canTakeOrder(*currOrder))){
                    currVolunteer->acceptOrder(*currOrder);
                    currOrder->setCollectorId(currVolunteer->getId());
                    currOrder->setStatus(OrderStatus::COLLECTING);
                    //check earase
                    inProcessOrders.push_back(currOrder); 
                    pendingOrders.erase(std::find(pendingOrders.begin(),pendingOrders.end(),currOrder)); 
                    stayInPlace = true;
                    break;      
                    }
                
                }
            }
        //if need to assign to a driver
        else{
            for (int j=0; j<(int)volunteers.size();j++){
                Volunteer *currVolunteer = volunteers[j];
                if((currVolunteer->getType()=="Driver" && currVolunteer->canTakeOrder(*currOrder))|| (currVolunteer->getType()=="LimitedDriver" &&currVolunteer->canTakeOrder(*currOrder))){
                    currVolunteer->acceptOrder(*currOrder);
                    currOrder->setDriverId(currVolunteer->getId());
                    currOrder->setStatus(OrderStatus::DELIVERING);
                    //check earase
                    inProcessOrders.push_back(currOrder);
                    pendingOrders.erase(std::find(pendingOrders.begin(),pendingOrders.end(),currOrder)); 
                    stayInPlace = true;
                    break;   
                    }     
                }
        }

        if(!stayInPlace)
            i++;
    }
    
    // Stage 2: Perform a step in the simulation
    for (Volunteer *volunteer : volunteers) {
    volunteer->step(); // Decrease time or distance left}
    }
    
    // Stage 3: Check if volunteers have finished processing orders
    for (int j=0; j<(int)volunteers.size(); j++) {    
        Volunteer *volunteer = volunteers[j];

        if (volunteer->getCompletedOrderId() != NO_ORDER) {
            if ((volunteer->getType()=="Collector") | (volunteer->getType()=="LimitedCollector")) {//waiting for driver
                int orderId = volunteer->getCompletedOrderId();
                Order *pendingForDriver=nullptr; 
                for(int m=0; m<(int)inProcessOrders.size();){
                    Order* order = inProcessOrders[m];
                    bool orderStayInPlace = false;
                    if(order->getId()==orderId){
                        pendingForDriver = order;
                        pendingOrders.push_back(pendingForDriver);
                        inProcessOrders.erase(std::find(inProcessOrders.begin(),inProcessOrders.end(),pendingForDriver));
                        orderStayInPlace = true;
                    } 
                    if(!orderStayInPlace)
                      m++;    
                 }
            }
            else {//finished
                int orderId = volunteer->getCompletedOrderId();
                Order *completedOrder=nullptr;
                for(int m=0; m<(int)pendingOrders.size();){
                    Order* order = pendingOrders[m];
                    bool orderStayInPlace = false;
                        if(order->getId()==orderId){
                            completedOrder = order;
                            completedOrder->setStatus(OrderStatus::COMPLETED);
                            completedOrders.push_back(completedOrder);
                            pendingOrders.erase(std::find(pendingOrders.begin(),pendingOrders.end(),completedOrder));
                            orderStayInPlace = true;
                        }
                    if(!orderStayInPlace)
                        m++;
                 }
            }
        }
    } 
        // Stage 4: Delete volunteers who reached maxOrders limit
        for (int j=0; j<(int)volunteers.size();) {
            bool volStayInPlace = false;
            Volunteer *currVolunteer = volunteers[j];
            if((currVolunteer->isBusy()==false) && (currVolunteer->hasOrdersLeft()==false)) {
                volunteers.erase(std::find(volunteers.begin(),volunteers.end(),currVolunteer));
                volStayInPlace = true;
                delete currVolunteer;
            }
            if(!volStayInPlace)
                j++;    
            }             
        }


void WareHouse::addOrder(Order* order){
    pendingOrders.push_back(order);
  //  increaseOrderCounter();
}
void WareHouse::addCustomer(Customer* customer){
    customers.push_back(customer);
    increaseCustomerCounter();
}
void WareHouse::addAction(BaseAction* action){ actionsLog.push_back(action);};

Customer &WareHouse::getCustomer(int customerId) const{ 
    Customer* custToReturn=nullptr;
    for (Customer* cust : customers){
            if(cust->getId()==customerId)
                custToReturn=cust;
    }
    return *custToReturn;
}
Volunteer &WareHouse::getVolunteer(int volunteerId) const{
    Volunteer* volToReturn=nullptr;
    for(Volunteer* vol:volunteers){
        if(vol->getId()==volunteerId)
            volToReturn=vol;
    }
    return *volToReturn;
}
Order &WareHouse::getOrder(int orderId) const{

    Order* orderToReturn=nullptr;

    for (Order *order : pendingOrders){
        if (order->getId() == orderId){
            return *order;
        }
    }
    for (Order *order : inProcessOrders){
        if (order->getId() == orderId){
            return *order;
        }
    }
    for (Order *order : completedOrders){
        if (order->getId() == orderId){
            return *order;
        }
    }
    return *orderToReturn;

}

OrderStatus WareHouse::OrderStatusById(int id){
    Order order = getOrder(id);
    OrderStatus status = order.getStatus();
    return status;
}

bool WareHouse::isVolunteerExist(int id){
    bool exist = false;
    for(Volunteer* vol:volunteers){
        if(vol->getId()==id)
           exist = true;}
    return exist;
}



const vector<BaseAction*> &WareHouse::getActions() const{return actionsLog;}
const vector<Volunteer*> &WareHouse::getVolunteersVector() const{return volunteers;}
const vector<Order*> &WareHouse::getPendingOrders()const{return pendingOrders;}
const vector<Order*> &WareHouse::getInProcessOrders()const{return inProcessOrders;}
const vector<Order*> &WareHouse::getCompletedOrders() const{return completedOrders;}
void WareHouse::close(){isOpen=false;}
void WareHouse::open(){isOpen=true; }
int WareHouse::getOrderCounter(){ return orderCounter;}
int WareHouse::getVolunteerCounter(){ return volunteerCounter;}
int WareHouse::getCustomerCounter(){ return customerCounter;}
int WareHouse::increaseOrderCounter(){orderCounter++; return orderCounter;}
int WareHouse::increaseCustomerCounter(){customerCounter++; return customerCounter;}