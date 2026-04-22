#include "../include/Employee.hpp"
#include <iostream>   
#include <iomanip>    
#include <algorithm>  
#include <string>     

// -------------------------------------------------------
Employee::Employee()
    : id(""), name(""), age(0), gender(""),
      position(""), salary(0.0), phone(""), email("")
{
    
}

// -------------------------------------------------------
Employee::Employee(
                   const std::string& id,
                   const std::string& name,
                   int age,
                   const std::string& gender,
                   const std::string& position,
                   double salary,
                   const std::string& phone,
                   const std::string& email
                )
    : id(id), name(name), age(age), gender(gender), position(position), salary(salary), phone(phone), email(email)
{
    
}

void Employee::displayRow() const {
    std::cout << "| "
              << std::left << std::setw(8)  << id        << " | "
              << std::left << std::setw(22) << name      << " | "
              << std::left << std::setw(4)  << age       << " | "
              << std::left << std::setw(7)  << gender    << " | "
              << std::left << std::setw(20) << position  << " | "
              << std::fixed << std::setprecision(2)
              << std::left << std::setw(12) << salary    << " | "
              << std::left << std::setw(14) << phone     << " | "
              << std::left << std::setw(28) << email     << " | " << "\n";
}

void Employee::displayDetails() const {
    std::cout << "\n====================================\n";
    std::cout << "         Employee Details\n";
    std::cout << "====================================\n";
    std::cout << std::left;
    std::cout << "  ID       : " << id       << "\n";
    std::cout << "  Name     : " << name     << "\n";
    std::cout << "  Age      : " << age      << "\n";
    std::cout << "  Gender   : " << gender   << "\n";
    std::cout << "  Position : " << position << "\n";
    std::cout << "  Salary   : " << std::fixed << std::setprecision(2) << salary << "\n";
    std::cout << "  Phone    : " << phone    << "\n";
    std::cout << "  Email    : " << email    << "\n";
    std::cout << "====================================\n";
}

// -------------------------------------------------------
json Employee::toJson() const {
    return json{
        {"id",       id},
        {"name",     name},
        {"age",      age},
        {"gender",   gender},
        {"position", position},
        {"salary",   salary},
        {"phone",    phone},
        {"email",    email}
    };
}

// -------------------------------------------------------
Employee Employee::fromJson(const json& j) {
    Employee emp;
    emp.id       = j.value("id",       "");
    emp.name     = j.value("name",     "");
    emp.age      = j.value("age",      0);
    emp.gender   = j.value("gender",   "");
    emp.position = j.value("position", "");
    emp.salary   = j.value("salary",   0.0);
    emp.phone    = j.value("phone",    "");
    emp.email    = j.value("email",    "");
    return emp;
}

// Note: Comparison is case-sensitive (IDs are always uppercase like "EMP001")
// -------------------------------------------------------
bool Employee::hasId(const std::string& searchId) const {
    return id == searchId;
}

// -------------------------------------------------------
bool Employee::nameContains(const std::string& searchTerm) const {
    // Make a lowercase copy of the employee's name
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(),
                   lowerName.begin(), ::tolower);

    // Make a lowercase copy of the search term
    std::string lowerTerm = searchTerm;
    std::transform(lowerTerm.begin(), lowerTerm.end(),
                   lowerTerm.begin(), ::tolower);

    // Return true if the lowercase name contains the lowercase search term
    return lowerName.find(lowerTerm) != std::string::npos;
}

// -------------------------------------------------------
void printTableHeader() {
    // Top border
    std::cout << "+-----------+------------------------+------+---------+"
              << "----------------------+---------------+-----------------+"
              << "------------------------------+\n";

    // Column labels row
    std::cout << "| "
              << std::left << std::setw(8)  << "ID"       << " | "
              << std::left << std::setw(22) << "Name"     << " | "
              << std::left << std::setw(4)  << "Age"      << " | "
              << std::left << std::setw(7)  << "Gender"   << " | "
              << std::left << std::setw(20) << "Position" << " | "
              << std::left << std::setw(12) << "Salary"   << " | "
              << std::left << std::setw(14) << "Phone"    << " | "
              << std::left << std::setw(28) << "Email"    << " |\n";

    // Header/body separator
    std::cout << "+-----------+------------------------+------+---------+"
              << "----------------------+---------------+-----------------+"
              << "------------------------------+\n";
}
// -------------------------------------------------------
void printTableFooter() {
    std::cout << "+-----------+------------------------+------+---------+"
              << "----------------------+---------------+-----------------+"
              << "------------------------------+\n";
}