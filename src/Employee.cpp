#include "../include/Employee.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <string>

Employee::Employee()
    : id(""), name(""), age(0), gender(""),
      position(""), salary(0.0), phone(""), email(""),
      username(""), password(""), role("employee")
{}

Employee::Employee(const std::string& id,
                   const std::string& name,
                   int age,
                   const std::string& gender,
                   const std::string& position,
                   double salary,
                   const std::string& phone,
                   const std::string& email,
                   const std::string& username,
                   const std::string& password,
                   const std::string& role)
    : id(id), name(name), age(age), gender(gender),
      position(position), salary(salary), phone(phone), email(email),
      username(username), password(password), role(role)
{}

std::vector<std::string> splitCSV(const std::string& line) {
    std::vector<std::string> fields;

    std::string cleanLine = line;
    while (!cleanLine.empty() &&
           (cleanLine.back() == '\r' || cleanLine.back() == '\n')) {
        cleanLine.pop_back();
    }

    std::stringstream ss(cleanLine);
    std::string token;

    while (std::getline(ss, token, ',')) {
        while (!token.empty() &&
               (token.back() == '\r' || token.back() == '\n')) {
            token.pop_back();
        }
        fields.push_back(token);
    }

    return fields;
}

std::string Employee::toCSV() const {
    std::ostringstream oss;
    oss << id       << ","
        << name     << ","
        << age      << ","
        << gender   << ","
        << position << ","
        << std::fixed << std::setprecision(2) << salary << ","
        << phone    << ","
        << email    << ","
        << username << ","
        << password << ","
        << role;
    return oss.str();
}

Employee Employee::fromCSV(const std::string& line) {
    std::vector<std::string> fields = splitCSV(line);

    Employee emp;

    if (fields.size() < 11) {
        return emp;
    }

    emp.id       = fields[0];
    emp.name     = fields[1];
    emp.age      = std::stoi(fields[2]);
    emp.gender   = fields[3];
    emp.position = fields[4];
    emp.salary   = std::stod(fields[5]);
    emp.phone    = fields[6];
    emp.email    = fields[7];
    emp.username = fields[8];
    emp.password = fields[9];
    emp.role     = fields[10];

    return emp;
}

bool Employee::isAdmin() const {
    return role == "admin";
}

bool Employee::matches(const std::string& inputUsername,
                       const std::string& inputPassword) const {
    return username == inputUsername && password == inputPassword;
}

void Employee::displayDetails() const {
    std::cout << "\n====================================\n";
    std::cout << "         Employee Details\n";
    std::cout << "====================================\n";
    std::cout << "  ID       : " << id       << "\n";
    std::cout << "  Name     : " << name     << "\n";
    std::cout << "  Age      : " << age      << "\n";
    std::cout << "  Gender   : " << gender   << "\n";
    std::cout << "  Position : " << position << "\n";
    std::cout << "  Salary   : " << std::fixed << std::setprecision(2) << salary << "\n";
    std::cout << "  Phone    : " << phone    << "\n";
    std::cout << "  Email    : " << email    << "\n";
    std::cout << "  Username : " << username << "\n";
    std::cout << "  Role     : " << role     << "\n";
    std::cout << "====================================\n";
}

bool Employee::hasId(const std::string& searchId) const {
    return id == searchId;
}

bool Employee::nameContains(const std::string& searchTerm) const {
    std::string lowerName = name;
    std::transform(lowerName.begin(), lowerName.end(),
                   lowerName.begin(), ::tolower);

    std::string lowerTerm = searchTerm;
    std::transform(lowerTerm.begin(), lowerTerm.end(),
                   lowerTerm.begin(), ::tolower);

    return lowerName.find(lowerTerm) != std::string::npos;
}