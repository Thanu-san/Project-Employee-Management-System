#pragma once

#include <string>
#include <vector>

class Report {
public:
    std::string from;     
    std::string date;     
    std::string subject;  
    std::string message;  

    Report();

    Report(const std::string& from,
           const std::string& date,
           const std::string& subject,
           const std::string& message);

    void displayReport() const;

    std::string toCSV() const;

    static Report fromCSV(const std::string& line);
};

std::string getTodayDate();

std::vector<std::string> splitReportCSV(const std::string& line);