// include/Employee.hpp
// -------------------------------------------------------
// Defines the Employee class: stores data for one employee and provides methods to display and convert to/from CSV.
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

    void displayDetails() const;

    // ---- CSV Conversion Methods ----

    std::string toCSV() const;

    // Creates an Employee object FROM one CSV line.

    static Employee fromCSV(const std::string& line);

    // ---- Comparison / Search ----

    bool hasId(const std::string& searchId) const;

    bool nameContains(const std::string& searchTerm) const;
};

// Helper: Split a CSV line into individual fields
// Declared here so Employee.cpp and other files can use it.

std::vector<std::string> splitCSV(const std::string& line);

// Helper: Print the table header row (column labels)
// Called once before printing multiple employee rows.

void printTableHeader();

// Helper: Print the table footer / separator line
// Called once after printing all employee rows.

void printTableFooter();