
#pragma once

#include <string>   
#include <vector>   

#include "Employee.hpp"  

class Manager {
private:
    std::vector<Employee> employees;

    Employee* currentUser;

    std::string employeeFile; 

    void saveEmployees();

    std::string generateNextId();

    bool idExists(const std::string& id);

    bool usernameExists(const std::string& username);

    void displayTable(const std::vector<Employee>& list);

    void clearScreen();

    void printHeader(const std::string& subtitle);

    void printMenuItem(const std::string& num,
                       const std::string& label);

    void printFooter();

    void pressEnterToContinue();

public:
    Manager(const std::string& employeeFile);

    void loadEmployees();

    void showLoginScreen();

    bool login(const std::string& username,
               const std::string& password);

    void logout();

    bool isLoggedIn() const;

    bool currentUserIsAdmin() const;

    std::string getCurrentUsername() const;

    void addEmployee();

    void viewEmployee();

    void editEmployee();

    void deleteEmployee();

    void searchEmployee();

    void showAdminMenu();

    void showEmployeeMenu();
};