#pragma once
#include <string>
#include <vector>
#include "Employee.hpp"
#include "Report.hpp"
#include "Attendance.hpp"

class Manager {
private:
    std::vector<Employee> employees;
    std::vector<Report> reports;
    std::vector<Attendance> attendances;
    Employee* currentUser;
    std::string employeeFile;
    std::string reportFile;
    std::string attendanceFile;
    std::string backupFolder;
    Employee hardcodedAdmin;

    void saveEmployees();
    void saveReports();
    void saveAttendances();
    std::string generateNextId();
    bool idExists(const std::string& id);
    bool usernameExists(const std::string& username);
    void displayTable(const std::vector<Employee>& list);
    void displayTablePaginated(const std::vector<Employee>& list);
    void clearScreen();
    void printHeader(const std::string& subtitle);
    void printMenuItem(const std::string& num, const std::string& label);
    void printFooter();
    void pressEnterToContinue();
    int getTerminalWidth();
    void printCentered(const std::string& text);
    std::string getInputSameLine();
    std::string getInputOptional();
    std::string getPasswordInput();
    void createBackup();
    void viewBackups();
    void restoreBackup();

public:
    Manager(const std::string& employeeFile,
            const std::string& reportFile,
            const std::string& attendanceFile,
            const std::string& backupFolder);

    void loadEmployees();
    void loadReports();
    void loadAttendances();

    void showLoginScreen();
    bool login(const std::string& username, const std::string& password);
    void signUp();
    void logout();
    bool isLoggedIn() const;
    bool currentUserIsAdmin() const;
    std::string getCurrentUsername() const;

    void addEmployee();
    void viewEmployee();
    void editEmployee();
    void deleteEmployee();
    void manageAttendance();
    void viewReports();
    void backupManagement();
    void searchEmployee();

    void viewMyProfile();
    void editMyProfile();
    void writeReport();
    void viewMyReports();
    void viewMyPayment();

    void showAdminMenu();
    void showEmployeeMenu();
};