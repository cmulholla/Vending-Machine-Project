// VendingMachine.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>

struct item {
    std::string name;
    float price;
};

class vendingMachine {
private:
    void serviceHelp();
    item items[50];
    std::string password;
public:
    vendingMachine() {
        // Display the following at the start of the program:
        std::cout << "Please Enter a command and its parameter" << std::endl;
        std::cout << "(Type Help for list of commands, Exit to quit)" << std::endl;
    }

    std::string serviceMode(std::string userPassword);
    std::string normalMode();
};

void vendingMachine::serviceHelp() {
    std::string commandList[] = {
        "Status\n    >prints the inventory",
        "Add [COLA | CUPS] <brand> <quantity>\n    >add pre-determined pop or cups to the machine",
        "Add | Remove [Coins | Bills] <denomination> <quantity>\n    >add money to the machine",
        "Exit\n    >exits the program",
        "Lock [password]\n    >locks the machine with a password and boots into NORMAL mode"
    };

    std::cout << "Commands in Service Mode are:" << std::endl;
    int size = sizeof(commandList) / sizeof(commandList[0]);
    for (int i = 0; i < size; i++) {
        std::cout << commandList[i] << std::endl;
    }
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

        //split the command into it's arguments
        if (command.find_first_of(" ") != -1) {
            args = command.substr(command.find_first_of(" ") + 1);
            command = command.substr(0, command.find_first_of(" "));
        }        

        // lowercase the command
        for (int i = 0; command[i]; i++) {
            command[i] = tolower(command[i]);
        }


        if (command == "help")
            serviceHelp();
        else if (command == "exit")
            return "";
        else if (command == "lock") {
            if (args == "") {
                std::cout << "lock usage: \n        Lock [password]" << std::endl;
            }
            else {
                return args;
            }
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

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
