// VendingMachine.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Vending Machine project is due March 15th

#include <iostream>
#include <string>
#include <fstream>

struct item {
    std::string name;
    float price;
    int amount;
};

std::string normalizeWhitespace(std::string command) {
    // fix all spaces so that there is:
    // no leading whitespace
    // no trailing whitespace
    // no more than 1 consecutive spaces in the middle of the command

    //fix leading whitespace
    while (command[0] == ' ') {
        command.erase(0, 1);
    }

    //fix trailing whitespace
    while (command.back() == ' ') {
        command.resize(command.size() - 1);
    }

    //copy the command to a new string skipping the consecutive whitespace
    std::string newCommand = "";
    int numConsecSpaces = 0;
    for (int i = 0; i < command.size(); i++) { //iterate through the string

        //keep track of consecutive spaces
        if (command[i] == ' ') {
            numConsecSpaces++;
        }
        else { //reset if no space char
            numConsecSpaces = 0;
        }

        //add char to string if it's not a consecutive space
        if (numConsecSpaces < 2) {
            newCommand += command[i];
        }
    }

    return newCommand;
}

class vendingMachine {
private:
    void serviceHelp(); //function that lists the functions
    void serviceStatus(std::string args);
    item items[50];
    item oneBills;
    item fiveBills;
    item pennies;
    item nickels;
    item dimes;
    item quarters;
    item cups;
    std::string password;
public:
    vendingMachine(); //init

    std::string serviceMode(std::string userPassword);  //puts the terminal into service mode. Returns the password, input has to be the same password.
    std::string normalMode();                           //puts the terminal into normal mode. Returns the password for input into service mode.
};

vendingMachine::vendingMachine() {
    // Display the following at the start of the program:
    std::cout << "Please Enter a command and its parameter" << std::endl;
    std::cout << "(Type Help for list of commands, Exit to quit)" << std::endl;

    // initialize the inventory items
    std::string line;
    std::ifstream inventoryFile("inventory.csv");
    std::string name, itemAmount;
    if (inventoryFile.is_open()) {
        while (std::getline(inventoryFile, line))
        {
            //split the line into it's parts
            if (line.find_first_of(",") != -1) {
                itemAmount = line.substr(line.find_first_of(",") + 1);
                name = line.substr(0, line.find_first_of(","));
            }

            //store the file items
            if (name == "One Dollar Bills")
                oneBills.amount = stoi(itemAmount);

            else if (name == "Five Dollar Bills")
                fiveBills.amount = stoi(itemAmount);

            else if (name == "Pennies")
                pennies.amount = stoi(itemAmount);

            else if (name == "Nickels")
                nickels.amount = stoi(itemAmount);

            else if (name == "Dimes")
                dimes.amount = stoi(itemAmount);

            else if (name == "Quarters")
                quarters.amount = stoi(itemAmount);
            
            else {

            }
        }
        inventoryFile.close();
    }

}

void vendingMachine::serviceHelp() {
    std::string commandList[] = {
        "Status",
        "Add [COLA | CUPS] <brand> <quantity>",
        "Add | Remove [Coins | Bills] <denomination> <quantity>",
        "Exit",
        "Lock [password]"
    };

    std::string commandDesc[] = {
        "prints the inventory",
        "add pre-determined pop or cups to the machine",
        "add money to the machine",
        "exits the program",
        "locks the machine with a password and boots into NORMAL mode"
    };

    std::cout << "Commands in Service Mode are:" << std::endl;
    int size = sizeof(commandList) / sizeof(commandList[0]);
    for (int i = 0; i < size; i++) {
        std::cout << commandList[i] << std::endl << "    >" << commandDesc[i] << std::endl << std::endl;
    }
}

void vendingMachine::serviceStatus(std::string args) {

}

std::string vendingMachine::serviceMode(std::string userPassword) {
    // TODO: comment (haha)
    if (password != userPassword) {
        std::cout << "Password does not match." << std::endl;
        return password;
    }

    std::string command, args;
    while (true) {
        std::cout << "[SERVICE MODE]>";
        std::getline(std::cin, command);

        command = normalizeWhitespace(command);

        //split the command into it's arguments
        if (command.find_first_of(" ") != -1) {
            args = command.substr(command.find_first_of(" ") + 1);
            command = command.substr(0, command.find_first_of(" "));
        }        

        // lowercase the command
        for (int i = 0; command[i]; i++) {
            command[i] = tolower(command[i]);
        }


        if (command == "help")          // help command
            serviceHelp();
        else if (command == "exit")     // exit command
            return "";
        else if (command == "lock") {   // lock command
            if (args == "") {
                std::cout << "lock usage: \n        Lock [password]" << std::endl;
            }
            else {
                return args;
            }
        }
        else if (command == "status") {

        }
            
    }
}

std::string vendingMachine::normalMode() {
    return "";
}

int main()
{

    vendingMachine machine;
    

    // Vending Machine starts in service mode
    std::string newPassword;
    newPassword = machine.serviceMode("");
    if (newPassword == "")
        return 0;
    else
        std::cout << "new password is: " << newPassword << std::endl;
    
}