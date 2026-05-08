#include "../include/Report.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>     

#ifdef _WIN32
    #include <windows.h>
#endif

std::string getTodayDate() {
    
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);

    std::ostringstream oss;
    oss << (now->tm_year + 1900) << "-"
        << std::setw(2) << std::setfill('0') << (now->tm_mon + 1) << "-"
        << std::setw(2) << std::setfill('0') << now->tm_mday;

    return oss.str();
}

std::vector<std::string> splitReportCSV(const std::string& line) {
    std::vector<std::string> fields;

    std::string cleanLine = line;
    while (!cleanLine.empty() &&
           (cleanLine.back() == '\r' || cleanLine.back() == '\n')) {
        cleanLine.pop_back();
    }

    std::stringstream ss(cleanLine);
    std::string token;

    while (std::getline(ss, token, '|')) {
        while (!token.empty() &&
               (token.back() == '\r' || token.back() == '\n')) {
            token.pop_back();
        }
        fields.push_back(token);
    }

    return fields;
}

Report::Report()
    : from(""), date(""), subject(""), message("")
{}

Report::Report(const std::string& from,
               const std::string& date,
               const std::string& subject,
               const std::string& message)
    : from(from), date(date), subject(subject), message(message)
{}

std::string Report::toCSV() const {
    std::ostringstream oss;
    oss << from    << "|"
        << date    << "|"
        << subject << "|"
        << message;
    return oss.str();
}

Report Report::fromCSV(const std::string& line) {
    std::vector<std::string> fields = splitReportCSV(line);

    Report r;
    if (fields.size() < 4) return r;

    r.from    = fields[0];
    r.date    = fields[1];
    r.subject = fields[2];
    r.message = fields[3];

    return r;
}

void Report::displayReport() const {
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

    // Color codes
    const std::string CYAN   = "\033[36m";
    const std::string YELLOW = "\033[33m";
    const std::string WHITE  = "\033[37m";
    const std::string GRAY   = "\033[90m";
    const std::string GREEN  = "\033[32m";
    const std::string RESET  = "\033[0m";

    std::cout << "\n";
    std::cout << p << CYAN << "+===========================================+" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << YELLOW << "              REPORT                      " << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "+===========================================+" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "From"    << RESET << " : " << WHITE  << std::setw(29) << from    << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "Date"    << RESET << " : " << GREEN  << std::setw(29) << date    << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << std::left << std::setw(9) << "Subject" << RESET << " : " << YELLOW << std::setw(29) << subject << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "+-------------------------------------------+" << RESET << "\n";
    std::cout << p << CYAN << "|" << RESET << "  " << GRAY << "Message  " << RESET << " : " << WHITE  << std::setw(29) << message << RESET << CYAN << "|" << RESET << "\n";
    std::cout << p << CYAN << "+===========================================+" << RESET << "\n";
}