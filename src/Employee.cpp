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
    int termWidth = 80;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        termWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#endif
    int pad = (termWidth - 55) / 2;
    std::string p(pad, ' ');

    // Color defines (same as Manager.cpp)
    const std::string CYAN   = "\033[36m";
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string WHITE  = "\033[37m";
    const std::string GRAY   = "\033[90m";
    const std::string RESET  = "\033[0m";

    std::cout << "\n";
    std::cout << p << CYAN << "+===========================================+" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << YELLOW << "           EMPLOYEE  PROFILE               " << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "+===========================================+" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "ID"       << RESET << " " << CYAN << ":" << RESET << "  " << WHITE << std::setw(28) << id       << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "Name"     << RESET << " " << CYAN << ":" << RESET << "  " << WHITE << std::setw(28) << name     << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "Age"      << RESET << " " << CYAN << ":" << RESET << "  " << WHITE << std::setw(28) << age      << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "Gender"   << RESET << " " << CYAN << ":" << RESET << "  " << WHITE << std::setw(28) << gender   << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "Position" << RESET << " " << CYAN << ":" << RESET << "  " << WHITE << std::setw(28) << position << RESET << CYAN << "|" << RESET << "\n";

    // Salary in green
    std::ostringstream salaryStream;
    salaryStream << std::fixed << std::setprecision(2) << salary;
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "Salary"   << RESET << " " << CYAN << ":" << RESET << "  " << GREEN << std::setw(28) << salaryStream.str() << RESET << CYAN << "|" << RESET << "\n";

    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "Phone"    << RESET << " " << CYAN << ":" << RESET << "  " << WHITE << std::setw(28) << phone    << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "Email"    << RESET << " " << CYAN << ":" << RESET << "  " << WHITE << std::setw(28) << email    << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "Username" << RESET << " " << CYAN << ":" << RESET << "  " << WHITE << std::setw(28) << username << RESET << CYAN << "|" << RESET << "\n";

    // Role in yellow
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "Role"     << RESET << " " << CYAN << ":" << RESET << "  " << YELLOW << std::setw(28) << role    << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "+===========================================+" << RESET << "\n";
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