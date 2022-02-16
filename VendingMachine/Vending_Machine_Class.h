#pragma once

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>

struct item {
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

std::string toLower(std::string str) { //lowercases the input string into the output string
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
    return str;
}

class vendingMachine { /////// Main Vending Machine Class ////////
private:
    ////// service mode functions //////
    void serviceHelp(std::string command = ""); // command that lists the functions.
    void serviceStatus(std::string args);       // command that lists the inventory.
    std::map<std::string, item> items;          // holds the inventory items.
    std::string itemNames[128];
    void setMapPrices(std::string itemName);    // initialization function meant to set prices in the items map.
    std::map<std::string, std::pair<std::string, std::string>> commandDesc; //5 commands, first being the short description and second being the long description.
    std::string password;                       // holds the password if in normal mode for redundancy.
    void addProduct(std::string args);          // command that adds pop or cups to the inventory
    void addSubCash(std::string args, bool add = true);          // command that adds or removes cash from the inventory

    
    ////// normal mode functions //////

public:
    vendingMachine();                           // init for inventory and help descriptions

    std::string serviceMode(std::string userPassword);  // puts the terminal into service mode. Returns the password, input has to be the same password.
    std::string normalMode();                           // puts the terminal into normal mode. Returns the password for input into service mode.
};

vendingMachine::vendingMachine() { //initialize the vending machine
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
            setMapPrices(name);
            
        }
        inventoryFile.close();
    }

    //initialize the descriptions of the functions
    //define all of the functions and their long descriptions
    std::string commandList[] = {
        "Status",
        "Add [COLA | CUPS] <brand> <quantity>",
        "Add | Remove [Coins | Bills] <denomination> <quantity>",
        "Exit",
        "Lock [password]"
    };

    std::string commandLongDesc[] = {
        "prints the inventory",
        "add pre-determined pop or cups to the machine",
        "add money to the machine",
        "exits the program",
        "locks the machine with a password and boots into NORMAL mode"
    };

    //set the descriptions to the map
    commandDesc["status"] = { commandList[0], commandLongDesc[0] };
    commandDesc["add"] = { commandList[1], commandLongDesc[1] };
    commandDesc["add | remove"] = { commandList[2], commandLongDesc[2] };
    commandDesc["exit"] = { commandList[3], commandLongDesc[3] };
    commandDesc["lock"] = { commandList[4], commandLongDesc[4] };

}

void vendingMachine::serviceHelp(std::string command) { //select a specific command to get help on, or leave blank to get all of the functions

    //output all of the function descriptions if there's no arguments
    if (command == "") {
        std::cout << "Commands in Service Mode are:" << std::endl;
        std::cout << commandDesc["status"].first << std::endl << "    >" << commandDesc["status"].second << std::endl << std::endl;
        std::cout << commandDesc["add"].first << std::endl << "    >" << commandDesc["add"].second << std::endl << std::endl;
        std::cout << commandDesc["add | remove"].first << std::endl << "    >" << commandDesc["add | remove"].second << std::endl << std::endl;
        std::cout << commandDesc["exit"].first << std::endl << "    >" << commandDesc["exit"].second << std::endl << std::endl;
        std::cout << commandDesc["lock"].first << std::endl << "    >" << commandDesc["lock"].second << std::endl << std::endl;
        return;
    }

    //if the command is specified, lowercase it
    command = toLower(command);

    //output only the specified command, if it exists
    if (commandDesc.count(command)) {
        std::cout << command << " command:" << std::endl;

        //output the description of the command
        std::cout << commandDesc[command].first << std::endl << "    >" << commandDesc[command].second << std::endl << std::endl;

        if (command == "add") // add extra functionality to the add help command, since there's two ways to use it.
            std::cout << commandDesc["add | remove"].first << std::endl << "    >" << commandDesc["add | remove"].second << std::endl << std::endl;
        return;
    }
    else //if the command does not exit, display a message
        std::cout << "Command does not exist." << std::endl;
}

void vendingMachine::serviceStatus(std::string args) {
    if (args != "") { // there should be no args, so display usage of the command if there are.
        serviceHelp("status");
    }

    // first, display the total amount of cash in the machine.
    
    std::cout << "Total Cash: $" << items["Five Dollar Bills"].amount * 5.0 +
        items["One Dollar Bills"].amount * 1.0 +
        items["Quarters"].amount * 0.25 +
        items["Dimes"].amount * 0.10 +
        items["Nickels"].amount * 0.05;

    // ouput the inventory file
    // TODO: make this better by outputting the map instead of the file, as the file will not be updated until the program is over (by file rewrite)
    std::ifstream inventoryFile("inventory.csv");
    std::string line, name, itemAmount;
    if (inventoryFile.is_open()) {
        while (std::getline(inventoryFile, line))
        {
            //split the line into it's parts
            if (line.find_first_of(",") != -1) {
                itemAmount = line.substr(line.find_first_of(",") + 1);
                name = line.substr(0, line.find_first_of(","));
            }
            std::cout << "Total " << name << ": " << itemAmount << std::endl;

        }
        inventoryFile.close();
    }
}

void vendingMachine::setMapPrices(std::string itemName)
{
    if (itemName == "One Dollar Bills") {
        items[itemName].price = 1;
    }
    else if (itemName == "Five Dollar Bills") {
        items[itemName].price = 5;
    }
    else if (itemName == "Nickels") {
        items[itemName].price = 0.05;
    }
    else if (itemName == "Dimes") {
        items[itemName].price = 0.1;
    }
    else if (itemName == "Quarters") {
        items[itemName].price = 0.25;
    }
    else if (itemName == "Cups") { // cannot buy a cup, so no cost.
        items[itemName].price = 0;
    }
    else { // anything else will be a type of pop, each costing $0.75
        items[itemName].price = 0.75;
    }
}

void vendingMachine::addProduct(std::string args) // Add [ Cola | Cups ] command
{
    try {
        if (args.substr(0, 4) == "cups") {
            items["Cups"].amount += stoi(args.substr(args.find_last_of(" ")));
        }
        std::cout << "There are now " << items["Cups"].amount << " cups." << std::endl;


    }
    catch (std::exception &err) {
        std::cout << "Please enter valid arguments." << std::endl;
    }
}

void vendingMachine::addSubCash(std::string args, bool add) // Add | Remove [Bills | Coins] command
{

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
        else // didn't find any arguments
            args = "";

        // lowercase the command
        command = toLower(command);


        if (command == "help")          // help command
            serviceHelp(args);
        else if (command == "exit")     // exit command
            return "";
        else if (command == "lock") {   // lock command
            if (args == "") { // output info on lock command if no password was given
                serviceHelp(command);
            }
            else { // if password was given, then return it. Main will boot into normal mode.
                password = args;
                return args;
            }
        }
        else if (command == "status") { // status command
            serviceStatus(args);
        }
        else if (command == "add") { // add and remove commands
            //parse the args to determine what the user is adding
            args = toLower(args); //lowercase the args
            if (args.substr(0, 4) == "cola" || args.substr(0, 4) == "cups")      // Add [ Cola | Cups ] command
                addProduct(args);
            else if (args.substr(0, 5) == "coins" || args.substr(0, 5) == "bills") // Add | Remove [ Bills | Coins ] command (just the add command part though)
                addSubCash(args, true);
            else
                std::cout << "Please enter valid arguments." << std::endl;
        }
        else if (command == "remove") { // Add | Remove command (just the remove command)
            addSubCash(args, false);
        }
        else {
            std::cout << "Invalid command." << std::endl;
        }


    }
}

std::string vendingMachine::normalMode() {
    return "";
}

