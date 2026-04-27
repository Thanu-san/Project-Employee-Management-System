#include "../include/Manager.hpp"
#include <iostream>   
#include <fstream>    
#include <sstream>    
#include <iomanip>    
#include <limits>     
#include <algorithm>  
#include <string>    
#include <vector>     

#include <tabulate/table.hpp>
using namespace tabulate;

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
    std::cout << "\n        Press Enter to continue...";
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Manager::printHeader(const std::string& subtitle) {
    std::cout << "\n";
    std::cout << "        +-----------------------------------+\n";
    std::cout << "        |   Employee Management System      |\n";
    std::cout << "        |  " << std::left << std::setw(33) << subtitle << "|\n";
    std::cout << "        +-----------------------------------+\n";
}

void Manager::printMenuItem(const std::string& num,
                            const std::string& label) {
    std::string item = num + ". " + label;
    std::cout << "        |   " << std::left << std::setw(33) << item << "|\n";
}

void Manager::printFooter() {
    std::cout << "        +-----------------------------------+\n";
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

    Table table;

    table.add_row({"ID", "Name", "Age", "Gender",
                   "Position", "Salary", "Phone", "Email",
                   "Username", "Role"});

    for (const Employee& emp : list) {
        table.add_row({
            emp.id,
            emp.name,
            std::to_string(emp.age),
            emp.gender,
            emp.position,
            std::to_string(emp.salary),
            emp.phone,
            emp.email,
            emp.username,
            emp.role
        });
    }

    table[0].format()
        .font_style({FontStyle::bold})
        .font_color(Color::cyan);

    std::cout << "\n" << table << "\n";
}

void Manager::showLoginScreen() {
    std::string username, password;

    while (true) {
        clearScreen();

        std::cout << "\n";
        std::cout << "+-----------------------------------+\n";
        std::cout << "|   Employee Management System      |\n";
        std::cout << "|        Welcome! Please Login      |\n";
        std::cout << "+-----------------------------------+\n";        
        std::cout << "\n";
        std::cout << "        Username : ";
        std::cin  >> username;
        std::cout << "        Password : ";
        std::cin  >> password;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (login(username, password)) {
            std::cout << "\n        Login successful! Welcome, "
                      << getCurrentUsername() << "!\n";
            pressEnterToContinue();
            return;
        } else {
            std::cout << "\n        Invalid username or password! Try again.\n";
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

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\n        Enter Employee Details:\n\n";

    std::cout << "        Name     : "; std::getline(std::cin, name);
    std::cout << "        Age      : "; std::cin >> age;     std::cin.ignore();
    std::cout << "        Gender   : "; std::getline(std::cin, gender);
    std::cout << "        Position : "; std::getline(std::cin, position);
    std::cout << "        Salary   : "; std::cin >> salary;  std::cin.ignore();
    std::cout << "        Phone    : "; std::getline(std::cin, phone);
    std::cout << "        Email    : "; std::getline(std::cin, email);

    std::cout << "\n        Enter Login Credentials:\n\n";

    while (true) {
        std::cout << "        Username : "; std::getline(std::cin, username);
        if (usernameExists(username)) {
            std::cout << "        Username already taken! Try another.\n";
        } else {
            break;
        }
    }

    std::cout << "        Password : "; std::getline(std::cin, password);

    std::cout << "        Role (admin / employee) : ";
    std::getline(std::cin, role);
    if (role != "admin" && role != "employee") {
        std::cout << "        Invalid role! Defaulting to 'employee'.\n";
        role = "employee";
    }

    std::string newId = generateNextId();

    Employee emp(newId, name, age, gender, position, salary,
                 phone, email, username, password, role);
    employees.push_back(emp);
    saveEmployees();

    std::cout << "\n        Employee added successfully! ID: " << newId << "\n";
    pressEnterToContinue();
}

void Manager::viewEmployee() {
    int choice;

    while (true) {
        clearScreen();
        printHeader("View Employee");
        printMenuItem("1", "View All");
        printMenuItem("2", "Sort by ID");
        printMenuItem("3", "Sort by Salary");
        printMenuItem("0", "Back");
        printFooter();

        std::cout << "\n        Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

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
            std::cout << "\n        Invalid choice! Try again.\n";
            pressEnterToContinue();
        }
    }
}

void Manager::editEmployee() {
    clearScreen();
    printHeader("Edit Employee");
    printFooter();

    std::string searchId;
    std::cout << "\n        Enter Employee ID to edit: ";
    std::cin >> searchId;
    std::cin.ignore();

    for (Employee& emp : employees) {
        if (emp.hasId(searchId)) {
            emp.displayDetails();

            std::cout << "\n        Enter new details (press Enter to keep current):\n\n";

            std::string newName;
            std::cout << "        New Name [" << emp.name << "] : ";
            std::getline(std::cin, newName);
            if (!newName.empty()) emp.name = newName;

            std::string newPosition;
            std::cout << "        New Position [" << emp.position << "] : ";
            std::getline(std::cin, newPosition);
            if (!newPosition.empty()) emp.position = newPosition;

            std::string newSalaryStr;
            std::cout << "        New Salary [" << emp.salary << "] : ";
            std::getline(std::cin, newSalaryStr);
            if (!newSalaryStr.empty()) {
                try {
                    emp.salary = std::stod(newSalaryStr);
                } catch (...) {
                    std::cout << "        Invalid salary, keeping original.\n";
                }
            }

            saveEmployees();
            std::cout << "\n        Employee updated successfully!\n";
            pressEnterToContinue();
            return;
        }
    }

    std::cout << "\n        Employee with ID '" << searchId << "' not found.\n";
    pressEnterToContinue();
}

void Manager::deleteEmployee() {
    clearScreen();
    printHeader("Delete Employee");
    printFooter();

    std::string searchId;
    std::cout << "\n        Enter Employee ID to delete: ";
    std::cin >> searchId;
    std::cin.ignore();

    if (currentUser != nullptr && currentUser->hasId(searchId)) {
        std::cout << "\n        You cannot delete your own account!\n";
        pressEnterToContinue();
        return;
    }

    for (int i = 0; i < (int)employees.size(); i++) {
        if (employees[i].hasId(searchId)) {
            employees[i].displayDetails();

            std::cout << "\n        Are you sure? (y/n): ";
            char confirm;
            std::cin >> confirm;
            std::cin.ignore();

            if (confirm == 'y' || confirm == 'Y') {
                employees.erase(employees.begin() + i);
                saveEmployees();
                std::cout << "\n        Employee deleted successfully!\n";
            } else {
                std::cout << "\n        Deletion cancelled.\n";
            }

            pressEnterToContinue();
            return;
        }
    }

    std::cout << "\n        Employee with ID '" << searchId << "' not found.\n";
    pressEnterToContinue();
}

void Manager::searchEmployee() {
    int choice;

    while (true) {
        clearScreen();
        printHeader("Search Employee");
        printMenuItem("1", "Search by ID");
        printMenuItem("2", "Search by Name");
        printMenuItem("0", "Back");
        printFooter();

        std::cout << "\n        Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 1) {
            clearScreen();
            printHeader("Search by ID");
            printFooter();

            std::string searchId;
            std::cout << "\n        Enter Employee ID: ";
            std::cin >> searchId;
            std::cin.ignore();

            bool found = false;
            for (const Employee& emp : employees) {
                if (emp.hasId(searchId)) {
                    emp.displayDetails();
                    found = true;
                    break;
                }
            }

            if (!found) {
                std::cout << "\n        No employee found with ID '"
                          << searchId << "'.\n";
            }

            pressEnterToContinue();

        } else if (choice == 2) {
            clearScreen();
            printHeader("Search by Name");
            printFooter();

            std::string searchTerm;
            std::cout << "\n        Enter name to search: ";
            std::getline(std::cin, searchTerm);

            std::vector<Employee> results;
            for (const Employee& emp : employees) {
                if (emp.nameContains(searchTerm)) {
                    results.push_back(emp);
                }
            }

            if (results.empty()) {
                std::cout << "\n        No employee found with name containing '"
                          << searchTerm << "'.\n";
            } else {
                std::cout << "\n        Found " << results.size() << " result(s):\n";
                displayTable(results);
            }

            pressEnterToContinue();

        } else if (choice == 0) {
            break;
        } else {
            std::cout << "\n        Invalid choice! Try again.\n";
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
        printMenuItem("1", "Add Employee");
        printMenuItem("2", "View Employee");
        printMenuItem("3", "Edit Employee");
        printMenuItem("4", "Delete Employee");
        printMenuItem("5", "Search Employee");
        printMenuItem("0", "Logout");
        printFooter();

        std::cout << "\n        Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: addEmployee();    break;
            case 2: viewEmployee();   break;
            case 3: editEmployee();   break;
            case 4: deleteEmployee(); break;
            case 5: searchEmployee(); break;
            case 0:
                logout();
                clearScreen();
                std::cout << "\n        Logged out successfully!\n";
                pressEnterToContinue();
                return;
            default:
                std::cout << "\n        Invalid choice! Try again.\n";
                pressEnterToContinue();
        }
    }
}

void Manager::showEmployeeMenu() {
    int choice;

    while (true) {
        clearScreen();

        std::string subtitle = "Logged in as: " + getCurrentUsername() + " [EMPLOYEE]";
        printHeader(subtitle);
        printMenuItem("1", "View Employee");
        printMenuItem("2", "Search Employee");
        printMenuItem("0", "Logout");
        printFooter();

        std::cout << "\n        Enter choice: ";
        std::cin >> choice;
        std::cin.ignore();

        switch (choice) {
            case 1: viewEmployee();   break;
            case 2: searchEmployee(); break;
            case 0:
                logout();
                clearScreen();
                std::cout << "\n        Logged out successfully!\n";
                pressEnterToContinue();
                return;
            default:
                std::cout << "\n        Invalid choice! Try again.\n";
                pressEnterToContinue();
        }
    }
}