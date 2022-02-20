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

std::string caseInsensitiveStr(std::vector<std::string> itemNames, std::string nameToMatch) { // returns the closest match in itemNames to nameToMatch. Picks the first value if there's two strings are the same.
    for (int i = 0; i < itemNames.size(); i++)
    {
        if (toLower(itemNames[i]) == toLower(nameToMatch)) {
            return itemNames[i];
        }
    }
    // no item found, return a blank string.
    return "";
}

class vendingMachine { /////// Main Vending Machine Class ////////
private:
    ////// service mode functions //////
    void serviceHelp(std::string command = ""); // command that lists the service mode functions.
    void serviceStatus(std::string args);       // command that lists the inventory.
    void setMapPrices(std::string itemName);    // initialization function meant to set prices in the items map.
    void addProduct(std::string args);          // command that adds pop or cups to the inventory
    void addSubCash(std::string args, bool add = true);          // command that adds or removes cash from the inventory

    // service mode global variables
    std::map<std::string, item> items;          // holds the inventory items.
    std::vector<std::string> itemNames;         // list of item names to make it easier to sort through all of the items. These are case-sensitive.
    std::map<std::string, std::pair<std::string, std::string>> commandDesc; //5 commands, first being the short description and second being the long description.
    std::string password;                       // holds the password if in normal mode for redundancy.
    
    ////// normal mode functions //////
    void normalHelp(std::string command = "");  // command that lists the normal mode functions
    void insertCoin(std::string args);          // command that inserts coins into the machine. Adds value to cashInserted and adds coin into items.
    void insertBill(std::string args);          // command that inserts bills into the machine. Adds value to cashInserted and adds bill into items.
    void chooseCola(std::string args);          // command that allows user to buy a cola.
    void returnCash();                          // function only for returning the amount of cash to the customer equal to the cashInserted variable.

    // normal mode global variables
    std::map<std::string, std::pair<std::string, std::string>> commandDescNormal; //same as commandDesc, but holds descriptions for the normal mode functions instead.
    float cashInserted; // stores how much money the user has inputted into the machine.

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
    cashInserted = 0;
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
            itemNames.push_back(name);
            
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
        "Prints the inventory",
        "Add pre-determined pop or cups to the machine",
        "Add money to the machine\n    ex: Remove coins 25 7\n    This removes 7 quarters",
        "Exits the program",
        "Locks the machine with a password and boots into NORMAL mode"
    };

    //set the descriptions to the map
    commandDesc["status"] = { commandList[0], commandLongDesc[0] };
    commandDesc["add"] = { commandList[1], commandLongDesc[1] };
    commandDesc["add | remove"] = { commandList[2], commandLongDesc[2] };
    commandDesc["exit"] = { commandList[3], commandLongDesc[3] };
    commandDesc["lock"] = { commandList[4], commandLongDesc[4] };

    // normal mode help functions
    std::string commandListNormal[] = { 
        "Coin <value> where value is [5 | 10 | 25 | nickel | dime | quarter]",
        "Bill <value> where value is [1 | 5]",
        "Cola <value> where value is [coke | pepsi | rc | jolt | faygo]",
        "Exit",
        "Unlock [password]"
    };

    std::string commandLongDescNormal[] = {
        "Insert a coin into the machine",
        "Insert a bill into the machine",
        "Select a soda to buy",
        "Exits the program (why are we letting users exit the machine?)",
        "Unlocks the machine with a password and boots into SERVICE mode with the correct password"
    };

    //set the descriptions to the normal mode map
    commandDescNormal["coin"] = { commandListNormal[0], commandLongDescNormal[0] };
    commandDescNormal["bill"] = { commandListNormal[1], commandLongDescNormal[1] };
    commandDescNormal["cola"] = { commandListNormal[2], commandLongDescNormal[2] };
    commandDescNormal["exit"] = { commandListNormal[3], commandLongDescNormal[3] };
    commandDescNormal["unlock"] = { commandListNormal[4], commandLongDescNormal[4] };
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
        items["Nickels"].amount * 0.05
        << std::endl;

    // ouput the inventory
    for (int i = 0; i < itemNames.size(); i++) {
        std::cout << "Total " << itemNames[i] << ": " << items[itemNames[i]].amount << std::endl; // reminder; items is the inventory map, itemNames is a vector with item names.
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
            std::cout << "There are now " << items["Cups"].amount << " cups." << std::endl;
        }
        else { // has to be cola because this function won't get called otherwise.
            std::string brand = args.substr(5, args.find_last_of(" ") - 5); //brand name of item should be between position 5 and the last space.
            std::string itemFound = caseInsensitiveStr(itemNames, brand);   //find the case-sensitive item name.
            
            // check to make sure the brand isn't money.
            if (toLower(brand) == "pennies" ||
                toLower(brand) == "nickels" ||
                toLower(brand) == "dimes" ||
                toLower(brand) == "quarters") {
                std::cout << "I see what you're tring to do here. No, you can't do it this way.\nUse 'add coins [int denomination] [amount]' to do this." << std::endl;
                return;
            }

            // no item found
            if (itemFound == "") { //no item found with the input brand. ASK ABOUT THIS - I can take this out to add new brands of soda.
                std::cout << "No item found with the name '" << brand << "'." << std::endl;
                return;
                items[brand].amount = 0; //This code only executes if the professor says that I should do it like this.
                items[brand].price = 0.75;
                items[brand].amount += stoi(args.substr(args.find_last_of(" "))); //amount of items to add
                std::cout << "There are now " << items[itemFound].amount << " cups worth of " << brand << "." << std::endl;
                return;
            }

            items[itemFound].amount += stoi(args.substr(args.find_last_of(" "))); //amount of items to add
            std::cout << "There are now " << items[itemFound].amount << " cups worth of " << itemFound << "." << std::endl;
        }
    }
    catch (std::exception &err) { //catches if stoi() failed, meaning the user did not input a number.
        std::cout << "Please enter valid arguments." << std::endl;
    }
}

void vendingMachine::addSubCash(std::string args, bool add) // Add | Remove [Coins | Bills] <denomination> <quantity> command
{
    // TODO: add functionality so both 25 and 'quarters' are valid inputs. only 25 is available right now.
    try {
        std::string denomination;
        int amount = std::stoi(args.substr(args.find_last_of(" ")));
        if (toLower(args.substr(0, 5)) == "bills") { //add bills
            if (std::stoi(args.substr(6, args.find_last_of(" "))) == 1) { // one dollar bill
                denomination = "One Dollar Bills";
            }
            else if (std::stoi(args.substr(6, args.find_last_of(" "))) == 5) { // five dollar bills
                denomination = "Five Dollar Bills";
            }
            else {
                std::cout << "Please input a correct denomination. [1 | 5]" << std::endl;
                return;
            }
        }
        else { //has to be coins, as the function will not be called otherwise.
            if (std::stoi(args.substr(6, args.find_last_of(" "))) == 5) { // nickels
                denomination = "Nickels";
            }
            else if (std::stoi(args.substr(6, args.find_last_of(" "))) == 10) { // dimes
                denomination = "Dimes";
            }
            else if (std::stoi(args.substr(6, args.find_last_of(" "))) == 25) { // quarters
                denomination = "Quarters";
            }
        }


        if (add) { //add currency
            items[denomination].amount += amount;
            std::cout << "Added " << amount << " " << denomination << "." << std::endl;
        }
        else { //remove currency
            //if you attempt to remove more than there are, it just removes all of them.
            if (items[denomination].amount < amount) {
                std::cout << "Removed " << items[denomination].amount << " " << denomination << "." << std::endl;
                items[denomination].amount = 0;
            }
            else { //remove a specific amount of currency.
                items[denomination].amount -= amount;
                std::cout << "Removed " << amount << " " << denomination << "." << std::endl;
            }
        }
        std::cout << items[denomination].amount << " " << denomination << " currently in the machine." << std::endl;
    }
    catch (std::exception& err) {
        std::cout << "Please enter valid arguments." << std::endl;
    }
}

////// Normal Mode Functions ///////

void vendingMachine::normalHelp(std::string command) { //select a specific command to get help on, or leave blank to get all of the functions

    //output all of the function descriptions if there's no arguments
    if (command == "") {
        std::cout << "Commands in Normal Mode are:" << std::endl;
        std::cout << commandDescNormal["coin"].first << std::endl << "    >" << commandDescNormal["coin"].second << std::endl << std::endl;
        std::cout << commandDescNormal["bill"].first << std::endl << "    >" << commandDescNormal["bill"].second << std::endl << std::endl;
        std::cout << commandDescNormal["cola"].first << std::endl << "    >" << commandDescNormal["cola"].second << std::endl << std::endl;
        std::cout << commandDescNormal["exit"].first << std::endl << "    >" << commandDescNormal["exit"].second << std::endl << std::endl;
        std::cout << commandDescNormal["unlock"].first << std::endl << "    >" << commandDescNormal["unlock"].second << std::endl << std::endl;
        return;
    }

    //if the command is specified, lowercase it
    command = toLower(command);

    //output only the specified command, if it exists
    if (commandDescNormal.count(command)) {
        std::cout << command << " command:" << std::endl;

        //output the description of the command
        std::cout << commandDescNormal[command].first << std::endl << "    >" << commandDescNormal[command].second << std::endl << std::endl;
        return;
    }
    else //if the command does not exit, display a message
    std::cout << "Command does not exist." << std::endl;
}

void vendingMachine::insertCoin(std::string args) //user mode function to input coins into the machine
{
    args = toLower(args);
    if (args == "5" || args == "nickel") { //user inputs a nickel
        items["Nickels"].amount++;
        cashInserted += 0.05;
        std::cout << "Nickel inserted." << std::endl;
    }
    else if (args == "10" || args == "dime") { //user inputs a dime
        items["Dimes"].amount++;
        cashInserted += 0.10;
        std::cout << "Dime inserted." << std::endl;
    }
    else if (args == "25" || args == "quarter") { //user inputs a quarter
        items["Quarters"].amount++;
        cashInserted += 0.25;
        std::cout << "Quarter inserted." << std::endl;
    }
    else { //user did not enter a correct value.
        //ouput usage of the coin function
        std::cout << "incorrect input value." << std::endl;
        normalHelp("coin");
        return;
    }

    //output the total cash the user has input into the machine.
    std::cout << "Total cash: $" << cashInserted << std::endl;
}

void vendingMachine::insertBill(std::string args) //user mode function to insert bills into the machine
{
    args = toLower(args);
    if (args == "1") { //user inputs a $1 bill
        items["One Dollar Bills"].amount++;
        cashInserted += 1;
        std::cout << "One Dollar Bill inserted." << std::endl;
    }
    else if (args == "5") { //user inputs a $5 bill
        items["Five Dollar Bills"].amount++;
        cashInserted += 5;
        std::cout << "Five Dollar Bill inserted." << std::endl;
    }
    else { //user did not enter a correct value.
        //ouput usage of the bill function
        std::cout << "incorrect input value." << std::endl;
        normalHelp("bill");
        return;
    }

    //output the total cash the user has input into the machine.
    std::cout << "Total cash: $" << cashInserted << std::endl;
}

void vendingMachine::chooseCola(std::string args) 
{
    //choose a cola to trade cashInserted for cola. 
    //If there's not enough money, then ask for more. 
    //If they entered too much, then give the pop and output the extra cash.
    //If there isn't enough change in the machine, then give back the bills instead of the pop.

    // find the itemName for the pop they wanted to buy
    std::string popChosen = "";
    for (int i = 0; i < itemNames.size(); i++) {
        if (toLower(itemNames[i]) == toLower(args)) { //break out of the loop once the pop has been found, as well as set the name to the variable.
            popChosen = itemNames[i];
            break;
        }
    }

    //input pop still the default string, so the pop was not found.
    if (popChosen == "") {
        std::cout << "Pop not found." << std::endl;
        return;
    }

    //pop found, continue
    items[popChosen].amount--;

}

void vendingMachine::returnCash() //return the cash equal to the amount of cash in the cashInserted variable. do nothing if there's not enough change.
{
    std::vector<std::string> cashNames = { "Five Dollar Bills",
        "One Dollar Bills",
        "Quarters",
        "Dimes",
        "Nickels" };

    //find if there's enough change.
    float tempChange = 0;
    int cashNum = 0;
    while (tempChange != cashInserted) { //redo this with a for statement starting at highest denomination and working it's way lower
        cashNum = 0;
        while (items[cashNames[cashNum]].price > cashInserted || items[cashNames[cashNum]].amount == 0) { //find the denomination of cash in the machine closest to the amount of cash inserted. has to be a lower number than the cash inserted.
            cashNum++;
        }
        if (items[cashNames[cashNum]].amount * items[cashNames[cashNum]].price <= cashInserted) { //use all of the rest of this denomination
            tempChange += items[cashNames[cashNum]].amount * items[cashNames[cashNum]].price;
        }
        else { //sum of denomination is greater than item input
            //if I have 10 quarters and 0 $1 bills and I need to output $1.20, then output 4 quarters (and 2 dimes if I have them)
            while (tempChange < cashInserted) {
                tempChange += items[cashNames[cashNum]].price;
            }
            //it'll overflow by 1, so minus it off
            tempChange -= items[cashNames[cashNum]].price;
        }

    }

}

////// Public Functions //////

std::string vendingMachine::serviceMode(std::string userPassword) {
    // TODO: comment (haha)
    if (password != userPassword) {
        std::cout << "Invalid password, try again." << std::endl;
        return password;
    }

    std::string command, args;
    while (true) {
        std::cout << "[SERVICE MODE]>";
        std::getline(std::cin, command);

        if (command == "") // literlly nothing found. This is an edge case. go back to the start of the while loop
            continue;

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
    std::string command, args;
    while (true) {
        std::cout << "[NORMAL MODE]>";
        std::getline(std::cin, command);

        if (command == "") // literlly nothing inputted. This is an edge case. go back to the start of the while loop
            continue;

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
            normalHelp(args);
        else if (command == "exit")     // exit command
            return "";
        else if (command == "unlock") {   // lock command
            if (args == "") { // output info on lock command if no password was given
                normalHelp(command);
            }
            else { // if password was given, then return it. Main will boot into normal mode.
                return args;
            }
        }
        else if (command == "coin") { // coin command
            insertCoin(args);
        }
        else if (command == "bill") { // bill commands
            insertBill(args);
        }
        else if (command == "cola") { // cola command
            chooseCola(args);
        }
        else {
            std::cout << "Invalid command." << std::endl;
        }
    }
}

