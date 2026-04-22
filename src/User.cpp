#include "../include/User.hpp"
#include "../include/Employee.hpp"  
#include <iostream>  
#include <sstream>  

User::User()
    : username(""), password(""), role("user")
{
    
}

User::User(const std::string& username,
           const std::string& password,
           const std::string& role)
    : username(username), password(password), role(role)
{
}

bool User::isAdmin() const {
    return role == "admin";
}

bool User::matches(const std::string& inputUsername,
                   const std::string& inputPassword) const {
    return username == inputUsername && password == inputPassword;
}

std::string User::toCSV() const {
    std::ostringstream oss;
    oss << username << ","
        << password << ","
        << role;
    return oss.str();
}

User User::fromCSV(const std::string& line) {
    std::vector<std::string> fields = splitCSV(line);

    User u;

    if (fields.size() < 3) {
        return u;  
    }

    u.username = fields[0];
    u.password = fields[1];
    u.role     = fields[2];

    return u;
}

void User::displayDetails() const {
    std::cout << "\n========================\n";
    std::cout << "       User Info\n";
    std::cout << "========================\n";
    std::cout << "  Username : " << username << "\n";
    std::cout << "  Role     : " << role << "\n";
    std::cout << "========================\n";
}