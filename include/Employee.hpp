#pragma once
#include <string>
#include <vector>

class Employee {
public:
    std::string id;
    std::string name;
    int         age;
    std::string gender;
    std::string position;
    double      salary;
    std::string phone;
    std::string email;
    std::string username;
    std::string password;
    std::string role;     

    Employee();

    Employee(const std::string& id,
             const std::string& name,
             int age,
             const std::string& gender,
             const std::string& position,
             double salary,
             const std::string& phone,
             const std::string& email,
             const std::string& username,
             const std::string& password,
             const std::string& role);

    bool isAdmin() const;
    bool matches(const std::string& inputUsername,
                 const std::string& inputPassword) const;

    void displayDetails(bool showPassword = false) const;

    std::string toCSV() const;
    static Employee fromCSV(const std::string& line);

    bool hasId(const std::string& searchId) const;
    bool nameContains(const std::string& searchTerm) const;
};

std::vector<std::string> splitCSV(const std::string& line);