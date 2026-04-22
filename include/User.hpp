// include/User.hpp
// -------------------------------------------------------
// Defines the User class: stores login credentials and role for each user in the system.
// -------------------------------------------------------

#pragma once  
#include <string> 


// User Class
class User {
public:
    // ---- Data Members ----
    std::string username;  
    std::string password;  
    std::string role;      

    // ---- Constructors ----

    User();

    User(const std::string& username,
         const std::string& password,
         const std::string& role);

    // ---- Role Check Methods ----

    bool isAdmin() const;

    bool matches(const std::string& inputUsername,
                 const std::string& inputPassword) const;

    std::string toCSV() const;

    static User fromCSV(const std::string& line);

    // ---- Display Method ----

    void displayDetails() const;
};