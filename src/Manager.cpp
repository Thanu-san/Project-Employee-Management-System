#include "../include/Manager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <limits>
#include <algorithm>
#include <string>
#include <vector>
#include <conio.h>

#ifdef _WIN32
    #include <windows.h>
#endif

#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"
#define BOLD    "\033[1m"

Manager::Manager(const std::string& employeeFile)
    : employeeFile(employeeFile), currentUser(nullptr)
{
    hardcodedAdmin.username = "admin";      
    hardcodedAdmin.password = "admin123";   
    hardcodedAdmin.role     = "admin";
    hardcodedAdmin.name     = "System Admin";
    hardcodedAdmin.id       = "ADMIN";
    hardcodedAdmin.age      = 0;
    hardcodedAdmin.salary   = 0.0;

    loadEmployees();
}

void Manager::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
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
    std::string stripped;
    bool inEscape = false;
    for (char c : text) {
        if (c == '\033') inEscape = true;
        if (!inEscape) stripped += c;
        if (inEscape && c == 'm') inEscape = false;
    }
    int termWidth = getTerminalWidth();
    int padding = (termWidth - (int)stripped.size()) / 2;
    if (padding > 0) std::cout << std::string(padding, ' ');
    std::cout << text << "\n";
}

void Manager::pressEnterToContinue() {
    int pad = (getTerminalWidth() - 26) / 2;
    std::cout << "\n" << std::string(pad, ' ')
              << CYAN << "Press Enter to continue..."
              << RESET << std::flush;
    _getch();
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
                std::cout << "\b \b" << std::flush;
            }
        } else {
            input += ch;
            std::cout << ch << std::flush;
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
                std::cout << "\b \b" << std::flush;
            }
        } else {
            input += ch;
            std::cout << ch << std::flush;
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
    if (!file.is_open()) return;
    std::string line;
    std::getline(file, line); 
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Employee emp = Employee::fromCSV(line);
            if (emp.role == "admin" && emp.username != hardcodedAdmin.username) {
                emp.role = "employee";
            }
            employees.push_back(emp);
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
    if (username == hardcodedAdmin.username) return true;
    for (const Employee& emp : employees) {
        if (emp.username == username) return true;
    }
    return false;
}

void Manager::displayTable(const std::vector<Employee>& list) {
    if (list.empty()) {
        int padMsg = (getTerminalWidth() - 20) / 2;
        std::cout << "\n" << std::string(padMsg, ' ')
                  << RED << "No employees found." << RESET << "\n";
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

    auto printRow = [&](
        const std::string& id,   const std::string& name,
        const std::string& age,  const std::string& gender,
        const std::string& pos,  const std::string& salary,
        const std::string& phone,const std::string& email,
        const std::string& user, const std::string& role)
    {
        std::cout << "        |"
                  << " " << std::left << std::setw(w1)  << id     << " |"
                  << " " << std::left << std::setw(w2)  << name   << " |"
                  << " " << std::left << std::setw(w3)  << age    << " |"
                  << " " << std::left << std::setw(w4)  << gender << " |"
                  << " " << std::left << std::setw(w5)  << pos    << " |"
                  << " " << std::left << std::setw(w6)  << salary << " |"
                  << " " << std::left << std::setw(w7)  << phone  << " |"
                  << " " << std::left << std::setw(w8)  << email  << " |"
                  << " " << std::left << std::setw(w9)  << user   << " |"
                  << " " << std::left << std::setw(w10) << role   << " |\n";
    };

    std::cout << "\n";
    printSep('+', '+', '-', '+');
    std::cout << YELLOW;
    printRow("ID", "Name", "Age", "Gender", "Position",
             "Salary", "Phone", "Email", "Username", "Role");
    std::cout << RESET;
    printSep('+', '+', '=', '+');

    for (const Employee& emp : list) {
        std::ostringstream salaryStream;
        salaryStream << std::fixed << std::setprecision(2) << emp.salary;
        printRow(emp.id, emp.name, std::to_string(emp.age),
                 emp.gender, emp.position, salaryStream.str(),
                 emp.phone, emp.email, emp.username, emp.role);
        printSep('+', '+', '-', '+');
    }
}

void Manager::showLoginScreen() {
    std::string username, password;

    while (true) {
        clearScreen();

        std::cout << "\n\n";
        printCentered(std::string(CYAN) + "*********************************************" + RESET);
        printCentered(std::string(CYAN) + "*                                           *" + RESET);
        printCentered(std::string(CYAN) + "*      EMPLOYEE  MANAGEMENT  SYSTEM         *" + RESET);
        printCentered(std::string(CYAN) + "*                                           *" + RESET);
        printCentered(std::string(CYAN) + "*********************************************" + RESET);
        std::cout << "\n";
        printCentered(std::string(CYAN) + "+-----------------------------------+" + RESET);
        printCentered(std::string(CYAN) + "| [1]. Login                        |" + RESET);
        printCentered(std::string(CYAN) + "| [2]. Sign Up                      |" + RESET);
        printCentered(std::string(CYAN) + "| [0]. Exit                         |" + RESET);
        printCentered(std::string(CYAN) + "+-----------------------------------+" + RESET);

        std::string choiceStr;
        int pad = (getTerminalWidth() - 37) / 2;
        std::cout << "\n" << std::string(pad, ' ') << "Enter choice: " << GREEN << std::flush;
        choiceStr = getInputSameLine();
        std::cout << RESET;

        int choice = -1;
        try { choice = std::stoi(choiceStr); } catch (...) {}

        if (choice == 0) {
            clearScreen();
            std::cout << "\n\n";
            printCentered(std::string(CYAN) + "*********************************************" + RESET);
            printCentered(std::string(CYAN) + "*      EMPLOYEE  MANAGEMENT  SYSTEM         *" + RESET);
            printCentered(std::string(CYAN) + "*********************************************" + RESET);
            std::cout << "\n";
            int padMsg = (getTerminalWidth() - 26) / 2;
            std::cout << std::string(padMsg, ' ')
                      << CYAN << "Goodbye! See you next time!" << RESET << "\n\n";
            exit(0);

        } else if (choice == 2) {
            signUp();
            continue;

        } else if (choice != 1) {
            continue;
        }

        // Login
        clearScreen();
        std::cout << "\n\n";
        printCentered(std::string(CYAN) + "*********************************************" + RESET);
        printCentered(std::string(CYAN) + "*                Please Login               *" + RESET);
        printCentered(std::string(CYAN) + "*********************************************" + RESET);
        std::cout << "\n";

        int padInput = (getTerminalWidth() - 37) / 2;
        std::string pi(padInput, ' ');

        std::cout << pi << "Username : " << GREEN << std::flush;
        username = getInputSameLine();
        std::cout << RESET;

        std::cout << pi << "Password : " << GREEN << std::flush;
        password = getPasswordInput();
        std::cout << RESET;

        if (login(username, password)) {
            std::cout << "\n";
            int padMsg = (getTerminalWidth() - 30) / 2;
            std::cout << std::string(padMsg, ' ')
                      << GREEN << "Login successful! Welcome, "
                      << getCurrentUsername() << "!" << RESET << "\n";
            pressEnterToContinue();
            return;
        } else {
            int padMsg = (getTerminalWidth() - 30) / 2;
            std::cout << "\n" << std::string(padMsg, ' ')
                      << RED << "Invalid username or password!" << RESET << "\n";
            pressEnterToContinue();
        }
    }
}

void Manager::signUp() {
    clearScreen();
    std::cout << "\n\n";
    printCentered(std::string(CYAN) + "*********************************************" + RESET);
    printCentered(std::string(CYAN) + "*                  Sign Up                  *" + RESET);
    printCentered(std::string(CYAN) + "*********************************************" + RESET);

    std::string name, gender, position, phone, email;
    std::string username, password;
    int age = 0;
    double salary = 0.0;

    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');

    std::cout << "\n" << p << "Create Your Account\n\n";

    std::cout << p << "Name     : " << GREEN << std::flush;
    name = getInputSameLine();
    std::cout << RESET;

    while (true) {
        std::cout << p << "Age      : " << GREEN << std::flush;
        std::string ageStr = getInputSameLine();
        std::cout << RESET;
        try { age = std::stoi(ageStr); break; }
        catch (...) {
            std::cout << p << RED << "Invalid age! Try again.\n" << RESET;
        }
    }

    std::cout << p << "Gender   : " << GREEN << std::flush;
    gender = getInputSameLine();
    std::cout << RESET;

    std::cout << p << "Position : " << GREEN << std::flush;
    position = getInputSameLine();
    std::cout << RESET;

    while (true) {
        std::cout << p << "Salary   : " << GREEN << std::flush;
        std::string salaryStr = getInputSameLine();
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

    std::cout << "\n" << p << "Create Login Credentials:\n\n";

    while (true) {
        std::cout << p << "Username : " << GREEN << std::flush;
        username = getInputSameLine();
        std::cout << RESET;
        if (usernameExists(username)) {
            std::cout << p << RED << "Username already taken! Try another.\n" << RESET;
        } else break;
    }

    std::cout << p << "Password : " << GREEN << std::flush;
    password = getInputSameLine();
    std::cout << RESET;

    std::string role = "employee";
    std::string newId = generateNextId();

    Employee emp(newId, name, age, gender, position, salary,
                 phone, email, username, password, role);
    employees.push_back(emp);
    saveEmployees();

    std::string msg = "Account created! ID: " + newId + " | Role: employee";
    int padMsg = (getTerminalWidth() - (int)msg.size()) / 2;
    std::cout << "\n" << std::string(padMsg, ' ') << GREEN << msg << RESET << "\n";
    pressEnterToContinue();
}

bool Manager::login(const std::string& username,
                    const std::string& password) {
    if (hardcodedAdmin.matches(username, password)) {
        currentUser = &hardcodedAdmin;
        return true;
    }
    for (Employee& emp : employees) {
        if (emp.matches(username, password)) {
            currentUser = &emp;
            return true;
        }
    }
    return false;
}

void Manager::logout() { currentUser = nullptr; }
bool Manager::isLoggedIn() const { return currentUser != nullptr; }
bool Manager::currentUserIsAdmin() const {
    return currentUser != nullptr && currentUser->isAdmin();
}
std::string Manager::getCurrentUsername() const {
    if (currentUser != nullptr) return currentUser->username;
    return "";
}

void Manager::addEmployee() {
    clearScreen();
    std::cout << "\n\n";
    printCentered(std::string(CYAN) + "*********************************************" + RESET);
    printCentered(std::string(CYAN) + "*              Add New Employee             *" + RESET);
    printCentered(std::string(CYAN) + "*********************************************" + RESET);

    std::string name, gender, position, phone, email;
    std::string username, password, role;
    int age = 0;
    double salary = 0.0;

    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');

    std::cout << "\n" << p << "Enter Employee Details:\n\n";

    std::cout << p << "Name     : " << GREEN << std::flush;
    name = getInputSameLine();
    std::cout << RESET;

    while (true) {
        std::cout << p << "Age      : " << GREEN << std::flush;
        std::string ageStr = getInputSameLine();
        std::cout << RESET;
        try { age = std::stoi(ageStr); break; }
        catch (...) {
            std::cout << p << RED << "Invalid age! Try again.\n" << RESET;
        }
    }

    std::cout << p << "Gender   : " << GREEN << std::flush;
    gender = getInputSameLine();
    std::cout << RESET;

    std::cout << p << "Position : " << GREEN << std::flush;
    position = getInputSameLine();
    std::cout << RESET;

    while (true) {
        std::cout << p << "Salary   : " << GREEN << std::flush;
        std::string salaryStr = getInputSameLine();
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
        } else break;
    }

    std::cout << p << "Password : " << GREEN << std::flush;
    password = getInputSameLine();
    std::cout << RESET;

    while (true) {
        std::cout << p << "Role (admin/employee) : " << GREEN << std::flush;
        role = getInputSameLine();
        std::cout << RESET;
        if (role == "admin" || role == "employee") break;
        std::cout << p << RED << "Invalid! Enter 'admin' or 'employee'.\n" << RESET;
    }

    std::string newId = generateNextId();
    Employee emp(newId, name, age, gender, position, salary,
                 phone, email, username, password, role);
    employees.push_back(emp);
    saveEmployees();

    std::string msg = "Employee added successfully! ID: " + newId;
    int padMsg = (getTerminalWidth() - (int)msg.size()) / 2;
    std::cout << "\n" << std::string(padMsg, ' ') << GREEN << msg << RESET << "\n";
    pressEnterToContinue();
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
        int pad = (getTerminalWidth() - 38) / 2;
        std::cout << "\n" << std::string(pad, ' ') << "Enter choice: " << GREEN << std::flush;
        choiceStr = getInputSameLine();
        std::cout << RESET;
        try { choice = std::stoi(choiceStr); } catch (...) { choice = -1; }

        if (choice == 1) {
            clearScreen();
            std::cout << "\n\n";
            printCentered(std::string(CYAN) + "*********************************************" + RESET);
            printCentered(std::string(CYAN) + "*                  View All                 *" + RESET);
            printCentered(std::string(CYAN) + "*********************************************" + RESET);
            displayTable(employees);
            pressEnterToContinue();

        } else if (choice == 2) {
            clearScreen();
            std::cout << "\n\n";
            printCentered(std::string(CYAN) + "*********************************************" + RESET);
            printCentered(std::string(CYAN) + "*                Sort by ID                 *" + RESET);
            printCentered(std::string(CYAN) + "*********************************************" + RESET);
            std::vector<Employee> sorted = employees;
            std::sort(sorted.begin(), sorted.end(),
                [](const Employee& a, const Employee& b) {
                    return a.id < b.id;
                });
            displayTable(sorted);
            pressEnterToContinue();

        } else if (choice == 3) {
            clearScreen();
            std::cout << "\n\n";
            printCentered(std::string(CYAN) + "*********************************************" + RESET);
            printCentered(std::string(CYAN) + "*               Sort by Salary              *" + RESET);
            printCentered(std::string(CYAN) + "*********************************************" + RESET);
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
                      << RED << "Invalid choice! Try again." << RESET << "\n";
            pressEnterToContinue();
        }
    }
}

void Manager::editEmployee() {
    clearScreen();
    std::cout << "\n\n";
    printCentered(std::string(CYAN) + "*********************************************" + RESET);
    printCentered(std::string(CYAN) + "*               Edit Employee               *" + RESET);
    printCentered(std::string(CYAN) + "*********************************************" + RESET);

    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');

    std::cout << "\n" << p << "Enter Employee ID to edit: " << GREEN << std::flush;
    std::string searchId = getInputSameLine();
    std::cout << RESET;

    for (Employee& emp : employees) {
        if (emp.hasId(searchId)) {
            emp.displayDetails(currentUserIsAdmin());

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
                try { emp.salary = std::stod(newSalaryStr); }
                catch (...) {
                    int padMsg = (getTerminalWidth() - 30) / 2;
                    std::cout << std::string(padMsg, ' ')
                              << RED << "Invalid salary, keeping original." << RESET << "\n";
                }
            }

            saveEmployees();
            int padMsg = (getTerminalWidth() - 30) / 2;
            std::cout << "\n" << std::string(padMsg, ' ')
                      << GREEN << "Employee updated successfully!" << RESET << "\n";
            pressEnterToContinue();
            return;
        }
    }

    int padMsg = (getTerminalWidth() - 30) / 2;
    std::cout << "\n" << std::string(padMsg, ' ')
              << RED << "Employee with ID '" << searchId << "' not found." << RESET << "\n";
    pressEnterToContinue();
}

void Manager::deleteEmployee() {
    clearScreen();
    std::cout << "\n\n";
    printCentered(std::string(CYAN) + "*********************************************" + RESET);
    printCentered(std::string(CYAN) + "*              Delete Employee              *" + RESET);
    printCentered(std::string(CYAN) + "*********************************************" + RESET);

    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');

    std::cout << "\n" << p << "Enter Employee ID to delete: " << GREEN << std::flush;
    std::string searchId = getInputSameLine();
    std::cout << RESET;

    if (currentUser != nullptr && currentUser->hasId(searchId)) {
        int padMsg = (getTerminalWidth() - 36) / 2;
        std::cout << "\n" << std::string(padMsg, ' ')
                  << RED << "You cannot delete your own account!" << RESET << "\n";
        pressEnterToContinue();
        return;
    }

    for (int i = 0; i < (int)employees.size(); i++) {
        if (employees[i].hasId(searchId)) {
            employees[i].displayDetails(currentUserIsAdmin());

            std::cout << "\n" << p << "Are you sure? (y/n): " << GREEN << std::flush;
            std::string confirm = getInputSameLine();
            std::cout << RESET;

            if (confirm == "y" || confirm == "Y") {
                employees.erase(employees.begin() + i);
                saveEmployees();
                int padMsg = (getTerminalWidth() - 30) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                          << GREEN << "Employee deleted successfully!" << RESET << "\n";
            } else {
                int padMsg = (getTerminalWidth() - 20) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                          << YELLOW << "Deletion cancelled." << RESET << "\n";
            }

            pressEnterToContinue();
            return;
        }
    }

    int padMsg = (getTerminalWidth() - 30) / 2;
    std::cout << "\n" << std::string(padMsg, ' ')
              << RED << "Employee with ID '" << searchId << "' not found." << RESET << "\n";
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
        try { choice = std::stoi(choiceStr); } catch (...) { choice = -1; }

        if (choice == 1) {
            clearScreen();
            std::cout << "\n\n";
            printCentered(std::string(CYAN) + "*********************************************" + RESET);
            printCentered(std::string(CYAN) + "*                Search by ID               *" + RESET);
            printCentered(std::string(CYAN) + "*********************************************" + RESET);

            std::cout << "\n" << p << "Enter Employee ID: " << GREEN << std::flush;
            std::string searchId = getInputSameLine();
            std::cout << RESET;

            bool found = false;
            for (const Employee& emp : employees) {
                if (emp.hasId(searchId)) {
                    emp.displayDetails(currentUserIsAdmin());
                    found = true;
                    break;
                }
            }

            if (!found) {
                int padMsg = (getTerminalWidth() - 30) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                          << RED << "No employee found with ID '" << searchId << "'." << RESET << "\n";
            }
            pressEnterToContinue();

        } else if (choice == 2) {
            clearScreen();
            std::cout << "\n\n";
            printCentered(std::string(CYAN) + "*********************************************" + RESET);
            printCentered(std::string(CYAN) + "*               Search by Name              *" + RESET);
            printCentered(std::string(CYAN) + "*********************************************" + RESET);
            
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
                          << RED << "No employee found with name containing '" << searchTerm << "'." << RESET << "\n";
            } else {
                int padMsg = (getTerminalWidth() - 20) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                          << GREEN << "Found " << results.size() << " result(s):" << RESET << "\n";
                displayTable(results);
            }
            pressEnterToContinue();

        } else if (choice == 0) {
            break;
        } else {
            int padMsg = (getTerminalWidth() - 22) / 2;
            std::cout << "\n" << std::string(padMsg, ' ')
                      << RED << "Invalid choice! Try again." << RESET << "\n";
            pressEnterToContinue();
        }
    }
}

void Manager::showAdminMenu() {
    int choice;

    while (true) {
        clearScreen();
        std::string subtitle = "Welcome !, " + getCurrentUsername() + " [ADMIN]";
        printHeader(subtitle);
        printMenuItem("[1]", "Add Employee");
        printMenuItem("[2]", "View Employee");
        printMenuItem("[3]", "Edit Employee");
        printMenuItem("[4]", "Delete Employee");
        printMenuItem("[5]", "Search Employee");
        printMenuItem("[0]", "Logout");
        printFooter();

        std::string choiceStr;
        int pad = (getTerminalWidth() - 38) / 2;
        std::cout << "\n" << std::string(pad, ' ') << "Enter choice: " << GREEN << std::flush;
        choiceStr = getInputSameLine();
        std::cout << RESET;
        try { choice = std::stoi(choiceStr); } catch (...) { choice = -1; }

        switch (choice) {
            case 1: addEmployee();    break;
            case 2: viewEmployee();   break;
            case 3: editEmployee();   break;
            case 4: deleteEmployee(); break;
            case 5: searchEmployee(); break;
            case 0: {
                logout();
                clearScreen();
                int padMsg = (getTerminalWidth() - 24) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                          << GREEN << "Logged out successfully!" << RESET << "\n";
                pressEnterToContinue();
                return;
            }
            default: {
                int padMsg = (getTerminalWidth() - 22) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                          << RED << "Invalid choice! Try again." << RESET << "\n";
                pressEnterToContinue();
            }
        }
    }
}

void Manager::showEmployeeMenu() {
    int choice;

    while (true) {
        clearScreen();
        std::string subtitle = "Welcome !, " + getCurrentUsername() + " [EMPLOYEE]";
        printHeader(subtitle);
        printMenuItem("[1]", "View Employee");
        printMenuItem("[2]", "Search Employee");
        printMenuItem("[0]", "Logout");
        printFooter();

        std::string choiceStr;
        int pad = (getTerminalWidth() - 38) / 2;
        std::cout << "\n" << std::string(pad, ' ') << "Enter choice: " << GREEN << std::flush;
        choiceStr = getInputSameLine();
        std::cout << RESET;
        try { choice = std::stoi(choiceStr); } catch (...) { choice = -1; }

        switch (choice) {
            case 1: viewEmployee();   break;
            case 2: searchEmployee(); break;
            case 0: {
                logout();
                clearScreen();
                int padMsg = (getTerminalWidth() - 24) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                          << GREEN << "Logged out successfully!" << RESET << "\n";
                pressEnterToContinue();
                return;
            }
            default: {
                int padMsg = (getTerminalWidth() - 22) / 2;
                std::cout << "\n" << std::string(padMsg, ' ')
                          << RED << "Invalid choice! Try again." << RESET << "\n";
                pressEnterToContinue();
            }
        }
    }
}