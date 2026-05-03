#include "../include/Manager.hpp"
#include <iostream>   
#include <fstream>    
#include <sstream>    
#include <iomanip>    
#include <limits>     
#include <algorithm>  
#include <string>    
#include <vector>    
#include <clocale>   
#include <conio.h>
#ifdef _WIN32
    #include <windows.h>
#endif
#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define BOLD    "\033[1m"
#define WHITE   "\033[37m"

Manager::Manager(const std::string& employeeFile)
    : employeeFile(employeeFile), currentUser(nullptr)
{
    loadEmployees();
}

void Manager::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void Manager::pressEnterToContinue() {
    int pad = (getTerminalWidth() - 26) / 2;
    std::cout << "\n" << std::string(pad, ' ')
              << CYAN << "Press Enter to continue..."
              << RESET << std::flush;
    _getch();
}

void Manager::printHeader(const std::string& subtitle) {
    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');
    std::cout << "\n";
    std::cout << p << CYAN << "+------------------------------------+\n" << RESET;
    std::cout << p << CYAN << "|" << RESET << "                                    " << CYAN << "|\n" << RESET;
    std::cout << p << CYAN << "|   " << RESET << WHITE << std::left << std::setw(33) << subtitle << RESET << CYAN << "|\n" << RESET;
    std::cout << p << CYAN << "|" << RESET << "                                    " << CYAN << "|\n" << RESET;
    std::cout << p << CYAN << "+------------------------------------+\n" << RESET;
}

void Manager::printMenuItem(const std::string& num,
                            const std::string& label) {
    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');
    std::string item = num + ". " + label;
    std::cout << p << CYAN << "|   " << RESET
              << WHITE << std::left << std::setw(33) << item << RESET
              << CYAN << "|\n" << RESET;
}

void Manager::printFooter() {
    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');
    std::cout << p << CYAN << "+------------------------------------+\n" << RESET;
}

void Manager::loadEmployees() {
    employees.clear();

    std::ifstream file(employeeFile);
    if (!file.is_open()) {
        return;
    }

    std::string line;

    std::getline(file, line);

    while (std::getline(file, line)) {
        if (!line.empty()) {
            employees.push_back(Employee::fromCSV(line));
        }
    }

    file.close();
}

void Manager::saveEmployees() {
    std::ofstream file(employeeFile);

    file << "ID,Name,Age,Gender,Position,Salary,Phone,Email,Username,Password,Role\n";

    for (const Employee& emp : employees) {
        file << emp.toCSV() << "\n";
    }

    file.close();
}

std::string Manager::generateNextId() {
    int maxNum = 0;

    for (const Employee& emp : employees) {
        if (emp.id.size() > 3) {
            try {
                int num = std::stoi(emp.id.substr(3));
                if (num > maxNum) maxNum = num;
            } catch (...) {}
        }
    }

    std::ostringstream oss;
    oss << "EMP" << std::setw(3) << std::setfill('0') << (maxNum + 1);
    return oss.str();
}

bool Manager::idExists(const std::string& id) {
    for (const Employee& emp : employees) {
        if (emp.hasId(id)) return true;
    }
    return false;
}

bool Manager::usernameExists(const std::string& username) {
    for (const Employee& emp : employees) {
        if (emp.username == username) return true;
    }
    return false;
}

void Manager::displayTable(const std::vector<Employee>& list) {
    if (list.empty()) {
        std::cout << "\n        No employees found.\n";
        return;
    }

    const int w1  = 8;   
    const int w2  = 20;  
    const int w3  = 4;   
    const int w4  = 7;   
    const int w5  = 20;  
    const int w6  = 11;  
    const int w7  = 14;  
    const int w8  = 25;  
    const int w9  = 10;  
    const int w10 = 8;   

    auto printSep = [&](char left, char mid, char fill, char right) {
        std::cout << "        " << left;
        std::cout << std::string(w1  + 2, fill) << mid;
        std::cout << std::string(w2  + 2, fill) << mid;
        std::cout << std::string(w3  + 2, fill) << mid;
        std::cout << std::string(w4  + 2, fill) << mid;
        std::cout << std::string(w5  + 2, fill) << mid;
        std::cout << std::string(w6  + 2, fill) << mid;
        std::cout << std::string(w7  + 2, fill) << mid;
        std::cout << std::string(w8  + 2, fill) << mid;
        std::cout << std::string(w9  + 2, fill) << mid;
        std::cout << std::string(w10 + 2, fill) << right << "\n";
    };

    // Lambda to print one row
    auto printRow = [&](
        const std::string& id,
        const std::string& name,
        const std::string& age,
        const std::string& gender,
        const std::string& position,
        const std::string& salary,
        const std::string& phone,
        const std::string& email,
        const std::string& username,
        const std::string& role)
    {
        std::cout << "        |"
                  << " " << std::left << std::setw(w1)  << id       << " |"
                  << " " << std::left << std::setw(w2)  << name     << " |"
                  << " " << std::left << std::setw(w3)  << age      << " |"
                  << " " << std::left << std::setw(w4)  << gender   << " |"
                  << " " << std::left << std::setw(w5)  << position << " |"
                  << " " << std::left << std::setw(w6)  << salary   << " |"
                  << " " << std::left << std::setw(w7)  << phone    << " |"
                  << " " << std::left << std::setw(w8)  << email    << " |"
                  << " " << std::left << std::setw(w9)  << username << " |"
                  << " " << std::left << std::setw(w10) << role     << " |\n";
    };

    std::cout << "\n";

    printSep('+', '+', '-', '+');

    printRow("ID", "Name", "Age", "Gender", "Position",
         "Salary", "Phone", "Email", "Username", "Role");

    printSep('+', '+', '=', '+');

    for (const Employee& emp : list) {
        std::ostringstream salaryStream;
        salaryStream << std::fixed << std::setprecision(2) << emp.salary;

        printRow(
            emp.id,
            emp.name,
            std::to_string(emp.age),
            emp.gender,
            emp.position,
            salaryStream.str(),
            emp.phone,
            emp.email,
            emp.username,
            emp.role
        );

        printSep('+', '+', '-', '+');
    }
}
    std::string Manager::getInputSameLine() {
    std::string input;
    char ch;
    while (true) {
        ch = _getch();  
        if (ch == '\r' || ch == '\n') {
            if (!input.empty()) {
                std::cout << "\n";
                break;  
            }
            
        } else if (ch == '\b') {  
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b";  
            }
        } else {
            input += ch;
            std::cout << ch;  
        }
    }
    return input;
}


std::string Manager::getInputOptional() {
    std::string input;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') {
            std::cout << "\n";
            break;  
        } else if (ch == '\b') {
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b";
            }
        } else {
            input += ch;
            std::cout << ch;
        }
    }
    return input;
}

std::string Manager::getPasswordInput() {
    std::string input;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') {
            if (!input.empty()) {
                std::cout << "\n";
                break;
            }
        } else if (ch == '\b') {
            if (!input.empty()) {
                input.pop_back();
                std::cout << "\b \b" << std::flush;
            }
        } else {
            input += ch;
            std::cout << "*" << std::flush;  
        }
    }
    return input;
}

int Manager::getTerminalWidth() {
    int width = 80;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#endif
    return width;
}

void Manager::printCentered(const std::string& text) {
    int termWidth = getTerminalWidth();
    int padding = (termWidth - (int)text.size()) / 2;
    if (padding > 0) std::cout << std::string(padding, ' ');
    std::cout << text << "\n";
}

void Manager::showLoginScreen() {
    std::string username, password;

    while (true) {
        clearScreen();

        std::cout << "\n";
        printCentered(std::string(CYAN) + "+------------------------------------+" + RESET);
        printCentered(std::string(CYAN) + "|                                    |" + RESET);
        printCentered(std::string(CYAN) + "|     Employee Management System     |" + RESET);
        printCentered(std::string(CYAN) + "|                                    |" + RESET);
        printCentered(std::string(CYAN) + "+------------------------------------+" + RESET);
        printCentered(std::string(CYAN) + "|  [1]. Login                        |" + RESET);
        printCentered(std::string(CYAN) + "|  [0]. Exit                         |" + RESET);
        printCentered(std::string(CYAN) + "+------------------------------------+" + RESET);

        std::string choiceStr;
        int boxPad = (getTerminalWidth() - 45) / 2;
        std::cout << "\n" << std::string(boxPad, ' ') << "Enter choice: " << GREEN << std::flush;
        choiceStr = getInputSameLine();
        std::cout << RESET;
        
        int choice;
        try {
            choice = std::stoi(choiceStr);
        } catch (...) {
            choice = -1;
        }

        if (choice == 0) {
            clearScreen();
            int pad = (getTerminalWidth() - 26) / 2;
            std::cout << "\n\n" << std::string(pad, ' ')
                      << CYAN << "Goodbye! See you next time!"
                      << RESET << "\n\n";
            exit(0);
        }

        if (choice != 1) continue;

        clearScreen();
        int pad = (getTerminalWidth() - 33) / 2;
        std::string p(pad, ' ');


        std::cout << "\n" << p << "Username : " << GREEN << std::flush;
        username = getInputSameLine();
        std::cout << RESET;
        
        std::cout << p << "Password : " << GREEN << std::flush;
        password = getPasswordInput();
        std::cout << RESET;

        if (login(username, password)) {
            int padMsg = (getTerminalWidth() - 30) / 2;
            std::cout << "\n" << std::string(padMsg, ' ')
                      << GREEN << "Login successful! Welcome, "
                      << getCurrentUsername() << "!"
                      << RESET << "\n";
            pressEnterToContinue();
            return;

        } else {
            int pad = (getTerminalWidth() - 30) / 2;
            std::cout << "\n" << std::string(pad, ' ') 
                      << RED << "Invalid username or password!" 
                      << RESET << "\n";
            pressEnterToContinue();
        }
    }
}

bool Manager::login(const std::string& username,
                    const std::string& password) {
    for (Employee& emp : employees) {
        if (emp.matches(username, password)) {
            currentUser = &emp;
            return true;
        }
    }
    return false;
}

void Manager::logout() {
    currentUser = nullptr;
}

bool Manager::isLoggedIn() const {
    return currentUser != nullptr;
}

bool Manager::currentUserIsAdmin() const {
    return currentUser != nullptr && currentUser->isAdmin();
}

std::string Manager::getCurrentUsername() const {
    if (currentUser != nullptr) return currentUser->username;
    return "";
}

void Manager::addEmployee() {
    clearScreen();
    printHeader("Add New Employee");
    printFooter();

    std::string name, gender, position, phone, email;
    std::string username, password, role;
    int age;
    double salary;

    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');

    std::cout << "\n" << p << "Enter Employee Details:\n\n";

    std::cout << p << "Name     : " << GREEN << std::flush;
    name = getInputSameLine();
    std::cout << RESET;

    std::string ageStr;
    while (true) {
    std::cout << p << "Age      : " << GREEN << std::flush;
    ageStr = getInputSameLine();
    std::cout << RESET;
    try { age = std::stoi(ageStr); break; }
    catch (...) {
        std::cout << p << RED << "Invalid age! Try again.\n" << RESET;
        }
    }

    std::cout << p << "Gender   : "  << GREEN << std::flush;
    gender = getInputSameLine();
    std::cout << RESET;

    std::cout << p << "Position : " << GREEN << std::flush;
    position = getInputSameLine();
    std::cout << RESET;

    std::string salaryStr;
    while (true) {
    std::cout << p << "Salary   : " << GREEN << std::flush;
    salaryStr = getInputSameLine();
    std::cout << RESET;
    try { salary = std::stod(salaryStr); break; }
    catch (...) {
        std::cout << p << RED << "Invalid salary! Try again.\n" << RESET;
        }
    }

    std::cout << p << "Phone    : " << GREEN << std::flush;
    phone = getInputSameLine();
    std::cout << RESET;

    std::cout << p << "Email    : " << GREEN << std::flush;
    email = getInputSameLine();
    std::cout << RESET;

    std::cout << "\n" << p << "Enter Login Credentials:\n\n";

    while (true) {
    std::cout << p << "Username : " << GREEN << std::flush;
    username = getInputSameLine();
    std::cout << RESET;
    if (usernameExists(username)) {
        std::cout << p << RED << "Username already taken! Try another.\n" << RESET;
    } else {
        break;
    }
    }

    std::cout << p << "Password : "  << GREEN << std::flush;
    password = getInputSameLine();
    std::cout << RESET;

    while (true) {
    std::cout << p << "Role (admin/employee) : " << GREEN << std::flush;
    role = getInputSameLine();
    std::cout << RESET;
    if (role == "admin" || role == "employee") break;
    std::cout << p << RED << "Invalid role! Please enter 'admin' or 'employee'.\n" << RESET;
    }

    std::string newId = generateNextId();

    Employee emp(newId, name, age, gender, position, salary,
                 phone, email, username, password, role);
    employees.push_back(emp);
    saveEmployees();

    std::string msg = "Employee added successfully! ID: " + newId;
    int padMsg = (getTerminalWidth() - (int)msg.size()) / 2;
    std::cout << "\n" << std::string(padMsg, ' ') << GREEN << msg << RESET << "\n";
}

void Manager::viewEmployee() {
    int choice;

    while (true) {
        clearScreen();
        printHeader("View Employee");
        printMenuItem("[1]", "View All");
        printMenuItem("[2]", "Sort by ID");
        printMenuItem("[3]", "Sort by Salary");
        printMenuItem("[0]", "Back");
        printFooter();

        std::string choiceStr;
        int boxPad = (getTerminalWidth() - 37) / 2;
        std::cout << "\n" << std::string(boxPad, ' ') << "Enter choice: " << GREEN << std::flush;
        choiceStr = getInputSameLine();
        std::cout << RESET;

        try {
            choice = std::stoi(choiceStr);
        } catch (...) {
            choice = -1;  
        }

        if (choice == 1) {
            clearScreen();
            printHeader("All Employees");
            printFooter();
            displayTable(employees);
            pressEnterToContinue();

        } else if (choice == 2) {
            clearScreen();
            printHeader("Employees Sorted by ID");
            printFooter();

            std::vector<Employee> sorted = employees;
            std::sort(sorted.begin(), sorted.end(),
                [](const Employee& a, const Employee& b) {
                    return a.id < b.id;
                });

            displayTable(sorted);
            pressEnterToContinue();

        } else if (choice == 3) {
            clearScreen();
            printHeader("Employees Sorted by Salary");
            printFooter();

            std::vector<Employee> sorted = employees;
            std::sort(sorted.begin(), sorted.end(),
                [](const Employee& a, const Employee& b) {
                    return a.salary > b.salary;  
                });

            displayTable(sorted);
            pressEnterToContinue();

        } else if (choice == 0) {
            break;
        } else {
            int padMsg = (getTerminalWidth() - 22) / 2;
            std::cout << "\n" << std::string(padMsg, ' ')
                      << RED << "Invalid choice! Try again."
                      << RESET << "\n";
            pressEnterToContinue();
        }
    }
}

void Manager::editEmployee() {
    clearScreen();
    printHeader("Edit Employee");
    printFooter();

    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');

    std::cout << "\n" << p << "Enter Employee ID to edit: " << GREEN << std::flush;
    std::string searchId = getInputSameLine();
    std::cout << RESET;

    for (Employee& emp : employees) {
        clearScreen();
        printHeader("Edit Employee");
        printFooter();
        if (emp.hasId(searchId)) {
            emp.displayDetails();

            std::cout << "\n" << p << "Press Enter to keep current value.\n\n";

            std::cout << p << "New Name [" << emp.name << "] : " << GREEN << std::flush;
            std::string newName = getInputOptional();
            std::cout << RESET;
            if (!newName.empty()) emp.name = newName;

            std::cout << p << "New Position [" << emp.position << "] : " << GREEN << std::flush;
            std::string newPosition = getInputOptional();
            std::cout << RESET;
            if (!newPosition.empty()) emp.position = newPosition;

            std::cout << p << "New Salary [" << emp.salary << "] : " << GREEN << std::flush;
            std::string newSalaryStr = getInputOptional();
            std::cout << RESET;
            if (!newSalaryStr.empty()) {
            try {
                emp.salary = std::stod(newSalaryStr);
            } catch (...) {
                int padMsg = (getTerminalWidth() - 30) / 2;
                std::cout << std::string(padMsg, ' ')
                          << RED << "Invalid salary, keeping original.\n" << RESET;
                }
            }

            saveEmployees();
            int padMsg = (getTerminalWidth() - 30) / 2;
            std::cout << "\n" << std::string(padMsg, ' ')
                      << GREEN << "Employee updated successfully!"
                      << RESET << "\n";
            pressEnterToContinue();
            return;
        }
    }

    int padMsg = (getTerminalWidth() - 30) / 2;
    std::cout << "\n" << std::string(padMsg, ' ')
              << RED << "Employee with ID '" << searchId << "' not found."
              << RESET << "\n";
        pressEnterToContinue();
}

void Manager::deleteEmployee() {
    clearScreen();
    printHeader("Delete Employee");
    printFooter();

    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');

    std::cout << "\n" << p << "Enter Employee ID to delete: " << GREEN << std::flush;
    std::string searchId = getInputSameLine();
    std::cout << RESET;

    if (currentUser != nullptr && currentUser->hasId(searchId)) {
        int padMsg = (getTerminalWidth() - 36) / 2;
        std::cout << "\n" << std::string(padMsg, ' ')
                  << RED << "You cannot delete your own account!"
                  << RESET << "\n";
        pressEnterToContinue();
        return;
    }

    for (int i = 0; i < (int)employees.size(); i++) {
        if (employees[i].hasId(searchId)) {
            employees[i].displayDetails();

            std::cout << "\n" << p << "Are you sure? (y/n): " << GREEN << std::flush;
            std::string confirm = getInputSameLine();
            std::cout << RESET;

            if (confirm == "y" || confirm == "Y") {
                employees.erase(employees.begin() + i);
                saveEmployees();
                int padMsg = (getTerminalWidth() - 30) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                      << GREEN << "Employee deleted successfully!"
                      << RESET << "\n";
            } else {
                int padMsg = (getTerminalWidth() - 20) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                      << YELLOW << "Deletion cancelled."
                      << RESET << "\n";
            }

            pressEnterToContinue();
            return;
        }
    }

    int padMsg = (getTerminalWidth() - 30) / 2;
    std::cout << "\n" << std::string(padMsg, ' ')
              << RED << "Employee with ID '" << searchId << "' not found."
              << RESET << "\n";
    pressEnterToContinue();
}

void Manager::searchEmployee() {
    int choice;

    while (true) {
        clearScreen();
        printHeader("Search Employee");
        printMenuItem("[1]", "Search by ID");
        printMenuItem("[2]", "Search by Name");
        printMenuItem("[0]", "Back");
        printFooter();

        std::string choiceStr;
        int pad = (getTerminalWidth() - 38) / 2;
        std::string p(pad, ' ');
        std::cout << "\n" << p << "Enter choice: " << GREEN << std::flush;
        choiceStr = getInputSameLine();
        std::cout << RESET;

        try {
            choice = std::stoi(choiceStr);
        } catch (...) {
            choice = -1;  
        }

        if (choice == 1) {
            clearScreen();
            printHeader("Search by ID");
            printFooter();

            std::cout << "\n" << p << "Enter Employee ID: " << GREEN << std::flush;
            std::string searchId = getInputSameLine();
            std::cout << RESET;

            bool found = false;
            for (const Employee& emp : employees) {
                if (emp.hasId(searchId)) {
                    emp.displayDetails();
                    found = true;
                    break;
                }
            }

            if (!found) {
                int padMsg = (getTerminalWidth() - 30) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                          << RED << "No employee found with ID '"
                          << searchId << "'."
                          << RESET << "\n";
            }

            pressEnterToContinue();

        } else if (choice == 2) {
            clearScreen();
            printHeader("Search by Name");
            printFooter();

            std::cout << "\n" << p << "Enter name to search: " << GREEN << std::flush;
            std::string searchTerm = getInputSameLine();
            std::cout << RESET;
            
            std::vector<Employee> results;
            for (const Employee& emp : employees) {
                if (emp.nameContains(searchTerm)) {
                    results.push_back(emp);
                }
            }

        if (results.empty()) {
            int padMsg = (getTerminalWidth() - 40) / 2;
            std::cout << "\n" << std::string(padMsg, ' ')
                          << RED << "No employee found with name containing '"
                          << searchTerm << "'."
                          << RESET << "\n";
        } else {
            int padMsg = (getTerminalWidth() - 20) / 2;
            std::cout << "\n" << std::string(padMsg, ' ')
                          << GREEN << "Found " << results.size() << " result(s):"
                          << RESET << "\n";
            displayTable(results);
        }

            pressEnterToContinue();

        } else if (choice == 0) {
            break;
        } else {
            int padMsg = (getTerminalWidth() - 22) / 2;
            std::cout << "\n" << std::string(padMsg, ' ')
                      << RED << "Invalid choice! Try again."
                      << RESET << "\n";
            pressEnterToContinue();
        }
    }
}

void Manager::showAdminMenu() {
    int choice;

    while (true) {
        clearScreen();

        std::string subtitle = "Logged in as: " + getCurrentUsername() + " [ADMIN]";
        printHeader(subtitle);
        printMenuItem("[1]", "Add Employee");
        printMenuItem("[2]", "View Employee");
        printMenuItem("[3]", "Edit Employee");
        printMenuItem("[4]", "Delete Employee");
        printMenuItem("[5]", "Search Employee");
        printMenuItem("[0]", "Logout");
        printFooter();
        std::string choiceStr;
        int boxPad = (getTerminalWidth() - 37) / 2;
        std::cout << "\n" << std::string(boxPad, ' ') << "Enter choice: " << GREEN << std::flush;
        choiceStr = getInputSameLine();
        std::cout << RESET;

        try {
            choice = std::stoi(choiceStr);
        } catch (...) {
            choice = -1;  
        }

        switch (choice) {
            case 1: addEmployee();    break;
            case 2: viewEmployee();   break;
            case 3: editEmployee();   break;
            case 4: deleteEmployee(); break;
            case 5: searchEmployee(); break;
            case 0:{
                logout();
                clearScreen();
                int pad = (getTerminalWidth() - 24) / 2;
                std::cout << "\n" << std::string(pad, ' ') 
                          << GREEN << "Logged out successfully!" 
                          << RESET << "\n";
                pressEnterToContinue();
                return;
            }
            default: {
                int padMsg = (getTerminalWidth() - 22) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                          << RED << "Invalid choice! Try again."
                          << RESET << "\n";
                pressEnterToContinue();
            }
        }
    }
}

void Manager::showEmployeeMenu() {
    int choice;

    while (true) {
        clearScreen();

        std::string subtitle = "Logged in as: " + getCurrentUsername() + " [EMPLOYEE]";
        printHeader(subtitle);
        printMenuItem("[1]", "View Employee");
        printMenuItem("[2]", "Search Employee");
        printMenuItem("[0]", "Logout");
        printFooter();

        std::string choiceStr;
        int boxPad = (getTerminalWidth() - 37) / 2;
        std::cout << "\n" << std::string(boxPad, ' ') << "Enter choice: " << GREEN << std::flush;
        choiceStr = getInputSameLine();
        std::cout << RESET;

        try {
            choice = std::stoi(choiceStr);
        } catch (...) {
            choice = -1;  
        }

        switch (choice) {
            case 1: viewEmployee();   break;
            case 2: searchEmployee(); break;
            case 0:
                logout();
                clearScreen();
                std::cout << "\n        Logged out successfully!\n";
                pressEnterToContinue();
                return;
            default: {
                int padMsg = (getTerminalWidth() - 22) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                          << RED << "Invalid choice! Try again."
                          << RESET << "\n";
                pressEnterToContinue();
            }
        }
    }
}