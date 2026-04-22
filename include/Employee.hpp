// include/Employee.hpp
// -------------------------------------------------------
// Defines the Employee class: stores data for one employeeand provides methods to display and convert to/from CSV.
// -------------------------------------------------------

#pragma once 
#include <string>   
#include <vector>   

// Employee Class

class Employee {
public:
    // ---- Data Members ----
    std::string id;        
    std::string name;      
    int         age;       
    std::string gender;    
    std::string position;  
    double      salary;    
    std::string phone;     
    std::string email;     

    // ---- Constructors ----

    Employee();

    Employee(const std::string& id,
             const std::string& name,
             int age,
             const std::string& gender,
             const std::string& position,
             double salary,
             const std::string& phone,
             const std::string& email
            );

    // ---- Display Method ----

    void displayDetails() const;

    std::string toCSV() const;

    static Employee fromCSV(const std::string& line);

    // ---- Search Helpers ----

    bool hasId(const std::string& searchId) const;

    bool nameContains(const std::string& searchTerm) const;
};

std::vector<std::string> splitCSV(const std::string& line);