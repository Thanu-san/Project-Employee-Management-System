// include/Employee.hpp
// -------------------------------------------------------
// Defines the Employee class: stores BOTH employee data AND login credentials for each person in the system.
// -------------------------------------------------------

#pragma once  

#include <string>  
#include <vector>  

// Employee Class

class Employee {
public:
    // ---- Employee Data ----
    std::string id;        
    std::string name;      
    int         age;       
    std::string gender;    
    std::string position;  
    double      salary;    
    std::string phone;     
    std::string email;     

    // ---- Login Credential Fields ----
    std::string username;  
    std::string password;  
    std::string role;      

    // ---- Constructors ----

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

    // ---- Login / Role Methods ----

    bool isAdmin() const;

    bool matches(const std::string& inputUsername,
                 const std::string& inputPassword) const;

    void displayDetails() const;

    std::string toCSV() const;

    static Employee fromCSV(const std::string& line);

    // ---- Search Helpers ----

    bool hasId(const std::string& searchId) const;

    bool nameContains(const std::string& searchTerm) const;
};

std::vector<std::string> splitCSV(const std::string& line);