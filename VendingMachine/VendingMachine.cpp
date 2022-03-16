// VendingMachine.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Vending Machine project is due March 15th

#include <iostream>
#include <string>
#include <fstream>
#include "Vending_Machine_Class.h"

int main()
{
    vendingMachine machine; //vendingMachine class is defined in the 'Vending_Machine_Class.h' header file.
    
    std::string newPassword, userPassword = "";
    do {
        newPassword = machine.serviceMode(newPassword); //load the program into service mode
        if (newPassword == "")
            break;
        else
            std::cout << "new password is: " << newPassword << std::endl;

        // load the program into normal mode. 
        // If it outputs a password, the service mode will check it. 
        // If it outputs an empty string then the program will exit.
        userPassword = machine.normalMode();

    } while (newPassword != "" && userPassword != "");

    //save the vending machine inventory before the program closes
    machine.saveInventory();
    return 0;
}