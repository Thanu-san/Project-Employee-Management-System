#pragma once

#include <string>
#include <vector>

class Attendance {
public:
    std::string employeeId;    
    std::string employeeName;  
    std::string month;         
    int absentDays;    
    double baseSalary;    
    double deduction;     
    double actualPay;     

    Attendance();

    Attendance(const std::string& employeeId,
               const std::string& employeeName,
               const std::string& month,
               int absentDays,
               double baseSalary);

    void calculatePayment();

    void displayPayment() const;


    std::string toCSV() const;

    static Attendance fromCSV(const std::string& line);
};

std::string getCurrentMonth();