#ifndef WAREHOUSE_H
#define WAREHOUSE_H

#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
#include "../include/Order.h"
#include "../include/Customer.h"
#include "../include/Action.h"
#include "../include/Volunteer.h"


class BaseAction;
class Volunteer;

// Warehouse responsible for Volunteers, Customers Actions, and Orders.


class WareHouse {

    public:
        //rule of 5
        WareHouse(const string &configFilePath);
        WareHouse(WareHouse&& other);
        WareHouse(const WareHouse& other);
        WareHouse& operator=(const WareHouse& other); //copy assignment
        WareHouse& operator=(WareHouse&& other); //move assignment

        void start();
        void addOrder(Order* order);
        void addAction(BaseAction* action);
        Customer &getCustomer(int customerId) const;
        Volunteer &getVolunteer(int volunteerId) const;
        Order &getOrder(int orderId) const;
        const vector<BaseAction*> &getActions() const;
        void close();
        void open();
        int getOrderCounter();
        int getCustomerCounter();
        int getVolunteerCounter();
        int increaseOrderCounter();
        int increaseCustomerCounter();
        void addCustomer(Customer* customer);
        OrderStatus OrderStatusById(int id);
        bool isVolunteerExist(int id);
        const vector<Order*> &getPendingOrders() const;
        const vector<Order*> &getInProcessOrders() const;
        void parsingTxt(const string &configFilePath);
        const vector<Volunteer*> &getVolunteersVector() const;
        const vector<Order*> &getCompletedOrders() const;
        void step();
        virtual ~WareHouse();


    private:
        bool isOpen;
        vector<BaseAction*> actionsLog;
        vector<Volunteer*> volunteers;
        vector<Order*> pendingOrders;
        vector<Order*> inProcessOrders;
        vector<Order*> completedOrders;
        vector<Customer*> customers;
        int customerCounter; //For assigning unique customer IDs
        int volunteerCounter; //For assigning unique volunteer IDs
        int orderCounter; //For assigning unique order IDs
};

#endif