// include/Employee.hpp
// -------------------------------------------------------
// Defines the Employee class: stores data for one employee and provides methods to display and convert to/from JSON.
// -------------------------------------------------------

#pragma once  
#include <string>          
#include "../libs/json.hpp" 
using json = nlohmann::json;

// Employee Class

class Employee {
public:
    // ---- Data Members ----
    std::string id;        
    std::string name;      
    int age;       
    std::string gender;    
    std::string position;  
    double salary;    
    std::string phone;     
    std::string email;     

    // ---- Constructors ----

    // Default constructor: creates an empty Employee object.
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

    // ---- Display Methods ----

    void displayRow() const;

    // Prints a full, detailed "card" view of this employee.
    void displayDetails() const;

    // ---- JSON Conversion Methods ----

    json toJson() const;

    // Creates an Employee object FROM a JSON object.
    static Employee fromJson(const json& j);

    // ---- Comparison ----

    bool hasId(const std::string& searchId) const;

    bool nameContains(const std::string& searchTerm) const;
};

void printTableHeader();

void printTableFooter();