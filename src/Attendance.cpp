#include "../include/Attendance.hpp"
#include "../include/Employee.hpp"  
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>   

#ifdef _WIN32
    #include <windows.h>
#endif

std::string getCurrentMonth() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    std::ostringstream oss;
    oss << (now->tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (now->tm_mon + 1);

    return oss.str();
}

Attendance::Attendance()
    : employeeId(""), employeeName(""), month(""),
      absentDays(0), baseSalary(0.0),
      deduction(0.0), actualPay(0.0)
{}

Attendance::Attendance(const std::string& employeeId,
                       const std::string& employeeName,
                       const std::string& month,
                       int absentDays,
                       double baseSalary)
    : employeeId(employeeId), employeeName(employeeName),
      month(month), absentDays(absentDays), baseSalary(baseSalary),
      deduction(0.0), actualPay(0.0)
{
    calculatePayment();
}

void Attendance::calculatePayment() {
    double deductionPerDay = baseSalary / 30.0;
    deduction = deductionPerDay * absentDays;
    actualPay = baseSalary - deduction;

    if (actualPay < 0.0) actualPay = 0.0;
}

std::string Attendance::toCSV() const {
    std::ostringstream oss;
    oss << employeeId   << ","
        << employeeName << ","
        << month        << ","
        << absentDays   << ","
        << std::fixed << std::setprecision(2) << baseSalary << ","
        << std::fixed << std::setprecision(2) << deduction  << ","
        << std::fixed << std::setprecision(2) << actualPay;
    return oss.str();
}

Attendance Attendance::fromCSV(const std::string& line) {

    std::vector<std::string> fields = splitCSV(line);

    Attendance a;
    if (fields.size() < 7) return a;

    a.employeeId = fields[0];
    a.employeeName = fields[1];
    a.month = fields[2];
    a.absentDays = std::stoi(fields[3]);
    a.baseSalary = std::stod(fields[4]);
    a.deduction = std::stod(fields[5]);
    a.actualPay = std::stod(fields[6]);

    return a;
}

void Attendance::displayPayment() const {
    int termWidth = 80;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(
        GetStdHandle(STD_OUTPUT_HANDLE), &csbi)) {
        termWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    }
#endif
    int pad = (termWidth - 44) / 2;
    std::string p(pad, ' ');

    const std::string CYAN   = "\033[36m";
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string WHITE  = "\033[37m";
    const std::string GRAY   = "\033[90m";
    const std::string RED    = "\033[31m";
    const std::string RESET  = "\033[0m";

    std::cout << "\n";
    std::cout << p << CYAN << "+===========================================+" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << YELLOW << "           PAYMENT  SUMMARY               " << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "+===========================================+" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(11) << "Employee"   << RESET << " : " << WHITE  << std::setw(27) << employeeName                          << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(11) << "ID"         << RESET << " : " << WHITE  << std::setw(27) << employeeId                            << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(11) << "Month"      << RESET << " : " << GREEN  << std::setw(27) << month                                 << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "+-------------------------------------------+" << RESET << "\n";

    std::ostringstream baseSalaryStr;
    baseSalaryStr << std::fixed << std::setprecision(2) << baseSalary;
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(11) << "Base Salary" << RESET << " : " << WHITE << std::setw(27) << baseSalaryStr.str() << RESET << CYAN << "|" << RESET << "\n";

    std::string absentStr = std::to_string(absentDays) + " days";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(11) << "Absent"     << RESET << " : " << RED   << std::setw(27) << absentStr            << RESET << CYAN << "|" << RESET << "\n";

    std::ostringstream deductionStr;
    deductionStr << std::fixed << std::setprecision(2) << deduction;
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(11) << "Deduction"  << RESET << " : " << RED   << std::setw(27) << deductionStr.str()   << RESET << CYAN << "|" << RESET << "\n";

    std::cout << p << CYAN << "+-------------------------------------------+" << RESET << "\n";

    std::ostringstream actualPayStr;
    actualPayStr << std::fixed << std::setprecision(2) << actualPay;
    std::string payColor = (absentDays == 0) ? GREEN : YELLOW;
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(11) << "Actual Pay" << RESET << " : " << payColor << std::setw(27) << actualPayStr.str() << RESET << CYAN << "|" << RESET << "\n";

    std::cout << p << CYAN << "+===========================================+" << RESET << "\n";
}