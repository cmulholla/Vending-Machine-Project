// VendingMachine.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Vending Machine project is due March 15th

#include <iostream>
#include <string>
#include <fstream>
#include "Vending_Machine_Class.h"

int main()
{
    vendingMachine machine;
    

    // Vending Machine starts in service mode
    std::string newPassword, userPassword = "";
    do {
        newPassword = machine.serviceMode(newPassword);
        if (newPassword == "")
            return 0;
        else
            std::cout << "new password is: " << newPassword << std::endl;

        userPassword = machine.normalMode();
    } while (newPassword != "" && userPassword != "");
    
}