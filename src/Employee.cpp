#include "../include/Employee.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <windows.h>
#ifdef _WIN32
    #include <windows.h>
#endif

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
#ifdef _WIN32
    #include <windows.h>
#endif
    int termWidth = 80;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        termWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#endif
    int pad = (termWidth - 38) / 2;
    std::string p(pad, ' ');

    std::cout << "\n" << p << "====================================\n";
    std::cout << p << "         Employee Details\n";
    std::cout << p << "====================================\n";
    std::cout << p << "ID       : " << id       << "\n";
    std::cout << p << "Name     : " << name     << "\n";
    std::cout << p << "Age      : " << age      << "\n";
    std::cout << p << "Gender   : " << gender   << "\n";
    std::cout << p << "Position : " << position << "\n";
    std::cout << p << "Salary   : " << std::fixed << std::setprecision(2) << salary << "\n";
    std::cout << p << "Phone    : " << phone    << "\n";
    std::cout << p << "Email    : " << email    << "\n";
    std::cout << p << "Username : " << username << "\n";
    std::cout << p << "Role     : " << role     << "\n";
    std::cout << p << "====================================\n";
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