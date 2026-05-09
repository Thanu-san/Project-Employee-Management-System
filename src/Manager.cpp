#include "../include/Manager.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <vector>
#include <conio.h>
#include <ctime>

#ifdef _WIN32
    #include <windows.h>
    #include <direct.h>
#endif

#define RESET   "\033[0m"
#define CYAN    "\033[36m"
#define GREEN   "\033[32m"
#define RED     "\033[31m"
#define YELLOW  "\033[33m"
#define WHITE   "\033[37m"
#define GRAY    "\033[90m"

Manager::Manager(const std::string& employeeFile,
                 const std::string& reportFile,
                 const std::string& attendanceFile,
                 const std::string& backupFolder)
    : employeeFile(employeeFile), reportFile(reportFile),
      attendanceFile(attendanceFile), backupFolder(backupFolder),
      currentUser(nullptr)
{
    hardcodedAdmin.username = "admin";
    hardcodedAdmin.password = "admin123";
    hardcodedAdmin.role     = "admin";
    hardcodedAdmin.name     = "System Admin";
    hardcodedAdmin.id       = "ADMIN";
    hardcodedAdmin.age      = 0;
    hardcodedAdmin.salary   = 0.0;
#ifdef _WIN32
    _mkdir(backupFolder.c_str());
#endif
    loadEmployees();
    loadReports();
    loadAttendances();
}

void Manager::clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

int Manager::getTerminalWidth() {
    int width = 80;
#ifdef _WIN32
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi))
        width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
#endif
    return width;
}

void Manager::printCentered(const std::string& text) {
    std::string stripped;
    bool inEscape = false;
    for (char c : text) {
        if (c == '\033') inEscape = true;
        if (!inEscape) stripped += c;
        if (inEscape && c == 'm') inEscape = false;
    }
    int padding = (getTerminalWidth() - (int)stripped.size()) / 2;
    if (padding > 0) std::cout << std::string(padding, ' ');
    std::cout << text << "\n";
}

void Manager::pressEnterToContinue() {
    int pad = (getTerminalWidth() - 26) / 2;
    std::cout << "\n" << std::string(pad, ' ')
              << CYAN << "Press Enter to continue..." << RESET << std::flush;
    _getch();
}

void Manager::printHeader(const std::string& subtitle) {
    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');
    std::cout << "\n";
    std::cout << p << CYAN << "+====================================+\n" << RESET;
    std::cout << p << CYAN << "|" << RESET << "                                    " << CYAN << "|\n" << RESET;
    std::cout << p << CYAN << "|   " << RESET << WHITE << std::left << std::setw(33) << subtitle << RESET << CYAN << "|\n" << RESET;
    std::cout << p << CYAN << "|" << RESET << "                                    " << CYAN << "|\n" << RESET;
    std::cout << p << CYAN << "+====================================+\n" << RESET;
}

void Manager::printMenuItem(const std::string& num, const std::string& label) {
    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');
    std::string item = num + ". " + label;
    std::cout << p << CYAN << "|   " << RESET
              << WHITE << std::left << std::setw(33) << item << RESET
              << CYAN << "|\n" << RESET;
}

void Manager::printFooter() {
    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');
    std::cout << p << CYAN << "+====================================+\n" << RESET;
}

// ---- INPUT HELPERS ----
std::string Manager::getInputSameLine() {
    std::string input;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') {
            if (!input.empty()) { std::cout << "\n"; break; }
        } else if (ch == '\b') {
            if (!input.empty()) { input.pop_back(); std::cout << "\b \b" << std::flush; }
        } else { input += ch; std::cout << ch << std::flush; }
    }
    return input;
}

std::string Manager::getInputOptional() {
    std::string input;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') { std::cout << "\n"; break; }
        else if (ch == '\b') {
            if (!input.empty()) { input.pop_back(); std::cout << "\b \b" << std::flush; }
        } else { input += ch; std::cout << ch << std::flush; }
    }
    return input;
}

std::string Manager::getPasswordInput() {
    std::string input;
    char ch;
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') {
            if (!input.empty()) { std::cout << "\n"; break; }
        } else if (ch == '\b') {
            if (!input.empty()) { input.pop_back(); std::cout << "\b \b" << std::flush; }
        } else { input += ch; std::cout << "*" << std::flush; }
    }
    return input;
}

void Manager::loadEmployees() {
    employees.clear();
    std::ifstream file(employeeFile);
    if (!file.is_open()) return;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Employee emp = Employee::fromCSV(line);
            if (emp.role == "admin" && emp.username != hardcodedAdmin.username)
                emp.role = "employee";
            employees.push_back(emp);
        }
    }
    file.close();
}

void Manager::saveEmployees() {
    createBackup();
    std::ofstream file(employeeFile);
    file << "ID,Name,Age,Gender,Position,Salary,Phone,Email,Username,Password,Role\n";
    for (const Employee& emp : employees) file << emp.toCSV() << "\n";
    file.close();
}

void Manager::loadReports() {
    reports.clear();
    std::ifstream file(reportFile);
    if (!file.is_open()) 
    return;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
        if (!line.empty()) reports.push_back(Report::fromCSV(line));
    file.close();
}

void Manager::saveReports() {
    std::ofstream file(reportFile);
    file << "From|Date|Subject|Message\n";
    for (const Report& r : reports) file << r.toCSV() << "\n";
    file.close();
}

void Manager::loadAttendances() {
    attendances.clear();
    std::ifstream file(attendanceFile);
    if (!file.is_open()) 
    return;
    std::string line;
    std::getline(file, line);
    while (std::getline(file, line))
        if (!line.empty()) attendances.push_back(Attendance::fromCSV(line));
    file.close();
}

void Manager::saveAttendances() {
    std::ofstream file(attendanceFile);
    file << "EmployeeID,EmployeeName,Month,AbsentDays,BaseSalary,Deduction,ActualPay\n";
    for (const Attendance& a : attendances) file << a.toCSV() << "\n";
    file.close();
}

std::string Manager::generateNextId() {
    int maxNum = 0;
    for (const Employee& emp : employees) {
        if (emp.id.size() > 3) {
            try { int n = std::stoi(emp.id.substr(3)); if (n > maxNum) maxNum = n; }
            catch (...) {}
        }
    }
    std::ostringstream oss;
    oss << "EMP" << std::setw(3) << std::setfill('0') << (maxNum + 1);
    return oss.str();
}

bool Manager::idExists(const std::string& id) {
    for (const Employee& emp : employees) if (emp.hasId(id)) return true;
    return false;
}

bool Manager::usernameExists(const std::string& username) {
    if (username == hardcodedAdmin.username) return true;
    for (const Employee& emp : employees) if (emp.username == username) return true;
    return false;
}

void Manager::createBackup() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::ostringstream dateStr;
    dateStr << (now->tm_year+1900) << "-"
            << std::setw(2) << std::setfill('0') << (now->tm_mon+1) << "-"
            << std::setw(2) << std::setfill('0') << now->tm_mday;
    std::string backupFile = backupFolder + "/employees_" + dateStr.str() + ".csv";
    std::ifstream src(employeeFile);
    std::ofstream dst(backupFile);
    if (src.is_open() && dst.is_open()) dst << src.rdbuf();
    src.close(); 
    dst.close();
}

void Manager::viewBackups() {
    clearScreen();
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*              [ View Backups ]             *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";

    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');
    std::vector<std::string> backupFiles;

#ifdef _WIN32
    WIN32_FIND_DATA findData;
    std::string searchPath = backupFolder + "/*.csv";
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do { backupFiles.push_back(findData.cFileName); }
        while (FindNextFile(hFind, &findData));
        FindClose(hFind);
    }

#endif
    if (backupFiles.empty()) {
        int padMsg = (getTerminalWidth() - 20) / 2;
        std::cout << "\n" << std::string(padMsg, ' ') << RED << "No backups found." << RESET << "\n";
        pressEnterToContinue(); 
        return;
    }
    std::sort(backupFiles.begin(), backupFiles.end());
    std::cout << "\n" << p << YELLOW << "Available Backups:\n" << RESET;
    std::cout << p << std::string(36, '-') << "\n";
    for (int i = 0; i < (int)backupFiles.size(); i++) {
        std::string fullPath = backupFolder + "/" + backupFiles[i];
        std::ifstream bFile(fullPath);
        int count = -1; 
        std::string line;
        while (std::getline(bFile, line)) if (!line.empty()) count++;
        bFile.close();
        std::cout << p << WHITE << (i+1) << ". " << backupFiles[i]
                  << GRAY << "  (" << count << " employees)" << RESET << "\n";
    }
    std::cout << p << std::string(36, '-') << "\n";
    pressEnterToContinue();
}

void Manager::restoreBackup() {
    clearScreen();
        printCentered(std::string(CYAN)+"*********************************************"+RESET);
        printCentered(std::string(CYAN)+"*            [ Restore Backups ]            *"+RESET);
        printCentered(std::string(CYAN)+"*********************************************"+RESET);
        std::cout << "\n";

    int pad = (getTerminalWidth() - 38) / 2;
    std::string p(pad, ' ');
    std::vector<std::string> backupFiles;

#ifdef _WIN32
    WIN32_FIND_DATA findData;
    std::string searchPath = backupFolder + "/*.csv";
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind != INVALID_HANDLE_VALUE) {
        do { backupFiles.push_back(findData.cFileName); }
        while (FindNextFile(hFind, &findData));
        FindClose(hFind);
    }

#endif
    if (backupFiles.empty()) {
        int padMsg = (getTerminalWidth() - 20) / 2;
        std::cout << "\n" << std::string(padMsg, ' ') << RED << "No backups found." << RESET << "\n";
        pressEnterToContinue(); 
        return;
    }

    std::sort(backupFiles.begin(), backupFiles.end());
    std::cout << "\n" << p << YELLOW << "Available Backups:\n" << RESET;
    std::cout << p << std::string(36, '-') << "\n";

    for (int i = 0; i < (int)backupFiles.size(); i++) {
        std::string fullPath = backupFolder + "/" + backupFiles[i];
        std::ifstream bFile(fullPath);
        int count = -1; 
        std::string line;
        while (std::getline(bFile, line)) if (!line.empty()) count++;
        bFile.close();
        std::cout << p << WHITE << (i+1) << ". " << backupFiles[i]
                  << GRAY << "  (" << count << " employees)" << RESET << "\n";
    }

    std::cout << p << std::string(36, '-') << "\n";
    std::cout << "\n" << p << "Enter backup number: " << GREEN << std::flush;
    std::string numStr = getInputSameLine(); 
    std::cout << RESET;

    int num = -1;
    try { num = std::stoi(numStr); } catch (...) {}
    if (num < 1 || num > (int)backupFiles.size()) {
        int padMsg = (getTerminalWidth() - 20) / 2;
        std::cout << "\n" << std::string(padMsg, ' ') << RED << "Invalid selection." << RESET << "\n";
        pressEnterToContinue(); 
        return;
    }

    std::cout << "\n" << p << YELLOW << "Are you sure? Current data will be replaced! (y/n): " << GREEN << std::flush;
    std::string confirm = getInputSameLine(); 
    std::cout << RESET;
    if (confirm != "y" && confirm != "Y") {
        int padMsg = (getTerminalWidth() - 20) / 2;
        std::cout << "\n" << std::string(padMsg, ' ') << YELLOW << "Restore cancelled." << RESET << "\n";
        pressEnterToContinue(); 
        return;
    }

    std::string selectedFile = backupFolder + "/" + backupFiles[num-1];
    std::ifstream src(selectedFile);
    std::ofstream dst(employeeFile);

    if (src.is_open() && dst.is_open()) {
        dst << src.rdbuf(); 
        src.close(); 
        dst.close();
        loadEmployees();
        int padMsg = (getTerminalWidth() - 30) / 2;
        std::cout << "\n" << std::string(padMsg, ' ')
                  << GREEN << "Restored! " << employees.size() << " employees loaded." << RESET << "\n";
    } else {
        int padMsg = (getTerminalWidth() - 20) / 2;
        std::cout << "\n" << std::string(padMsg, ' ') << RED << "Restore failed!" << RESET << "\n";
    }
    pressEnterToContinue();
}

void Manager::displayTable(const std::vector<Employee>& list) {
    if (list.empty()) {
        int padMsg = (getTerminalWidth() - 20) / 2;
        std::cout << "\n" << std::string(padMsg, ' ') << RED << "No employees found." << RESET << "\n";
        return;
    }
    const int w1=8,w2=20,w3=4,w4=7,w5=20,w6=11,w7=14,w8=25,w9=10,w10=8;
    auto printSep = [&](char left, char mid, char fill, char right) {
        std::cout << "        " << left;
        for (int w : {w1,w2,w3,w4,w5,w6,w7,w8,w9,w10})
            std::cout << std::string(w+2, fill) << mid;
        std::cout << "\b" << right << "\n";
    };
    auto printRow = [&](const std::string& id, const std::string& name,
                        const std::string& age, const std::string& gender,
                        const std::string& pos, const std::string& salary,
                        const std::string& phone, const std::string& email,
                        const std::string& user, const std::string& role) {
        std::cout << "        |"
                  << " " << std::left << std::setw(w1)  << id     << " |"
                  << " " << std::left << std::setw(w2)  << name   << " |"
                  << " " << std::left << std::setw(w3)  << age    << " |"
                  << " " << std::left << std::setw(w4)  << gender << " |"
                  << " " << std::left << std::setw(w5)  << pos    << " |"
                  << " " << std::left << std::setw(w6)  << salary << " |"
                  << " " << std::left << std::setw(w7)  << phone  << " |"
                  << " " << std::left << std::setw(w8)  << email  << " |"
                  << " " << std::left << std::setw(w9)  << user   << " |"
                  << " " << std::left << std::setw(w10) << role   << " |\n";
    };
    std::cout << "\n";
    printSep('+','+','-','+');
    std::cout << YELLOW;
    printRow("ID","Name","Age","Gender","Position","Salary","Phone","Email","Username","Role");
    std::cout << RESET;
    printSep('+','+','=','+');
    for (const Employee& emp : list) {
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(2) << emp.salary;
        printRow(emp.id, emp.name, std::to_string(emp.age),
                 emp.gender, emp.position, ss.str(),
                 emp.phone, emp.email, emp.username, emp.role);
        printSep('+','+','-','+');
    }
}

void Manager::displayTablePaginated(const std::vector<Employee>& list) {
    if (list.empty()) {
        int padMsg = (getTerminalWidth() - 20) / 2;
        std::cout << "\n" << std::string(padMsg, ' ') << RED << "No employees found." << RESET << "\n";
        pressEnterToContinue(); 
        return;
    }
    const int PAGE_SIZE = 8;
    int totalPages = ((int)list.size() + PAGE_SIZE - 1) / PAGE_SIZE;
    int currentPage = 0;
    while (true) {
        clearScreen();
        int start = currentPage * PAGE_SIZE;
        int end   = std::min(start + PAGE_SIZE, (int)list.size());
        std::vector<Employee> pageList(list.begin()+start, list.begin()+end);
        int pad = (getTerminalWidth() - 38) / 2;
        std::string p(pad, ' ');
        std::cout << "\n" << p << YELLOW
                  << "Page " << (currentPage+1) << " of " << totalPages
                  << "  (" << list.size() << " total)" << RESET << "\n";
        displayTable(pageList);
        std::cout << "\n";
        std::cout << p << CYAN << "+-------------------------------+\n" << RESET;
        if (currentPage > 0)
            std::cout << p << CYAN << "|" << RESET << WHITE << "  [P] Previous Page        " << RESET << CYAN << "|\n" << RESET;
        if (currentPage < totalPages-1)
            std::cout << p << CYAN << "|" << RESET << WHITE << "  [N] Next Page            " << RESET << CYAN << "|\n" << RESET;
        std::cout << p << CYAN << "|" << RESET << WHITE << "  [0] Back                  " << RESET << CYAN << "|\n" << RESET;
        std::cout << p << CYAN << "+-------------------------------+\n" << RESET;
        std::cout << "\n" << p << "Enter choice: " << GREEN << std::flush;
        std::string choice = getInputSameLine(); std::cout << RESET;
        if (choice=="N"||choice=="n") { if (currentPage<totalPages-1) currentPage++; }
        else if (choice=="P"||choice=="p") { if (currentPage>0) currentPage--; }
        else if (choice=="0") break;
    }
}

void Manager::showLoginScreen() {
    std::string username, password;
    while (true) {
        clearScreen();
        std::cout << "\n\n";
        printCentered(std::string(CYAN)+"*********************************************"+RESET);
        printCentered(std::string(CYAN)+"*                                           *"+RESET);
        printCentered(std::string(CYAN)+"*      EMPLOYEE  MANAGEMENT  SYSTEM         *"+RESET);
        printCentered(std::string(CYAN)+"*                                           *"+RESET);
        printCentered(std::string(CYAN)+"*********************************************"+RESET);
        std::cout << "\n";
        printCentered(std::string(CYAN)+"+=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=+"+RESET);
        printCentered(std::string(CYAN)+"| [1]. Login                        |"+RESET);
        printCentered(std::string(CYAN)+"| [2]. Sign Up                      |"+RESET);
        printCentered(std::string(CYAN)+"| [0]. Exit                         |"+RESET);
        printCentered(std::string(CYAN)+"+=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=+"+RESET);

        std::string choiceStr;
        int pad = (getTerminalWidth()-37)/2;
        std::cout << "\n" << std::string(pad,' ') << "Enter choice: " << GREEN << std::flush;
        choiceStr = getInputSameLine(); std::cout << RESET;
        int choice = -1;
        try { choice = std::stoi(choiceStr); } catch (...) {}

        if (choice == 0) {
            clearScreen();
            std::cout << "\n\n";
            printCentered(std::string(CYAN)+"*********************************************"+RESET);
            printCentered(std::string(CYAN)+"*      EMPLOYEE  MANAGEMENT  SYSTEM         *"+RESET);
            printCentered(std::string(CYAN)+"*********************************************"+RESET);
            std::cout << "\n";
            int padMsg = (getTerminalWidth()-26)/2;
            std::cout << std::string(padMsg,' ') << CYAN << "Goodbye! See you !" << RESET << "\n\n";
            exit(0);
        } else if (choice == 2) { signUp(); continue; }
        else if (choice != 1) continue;
        clearScreen();
        printCentered(std::string(CYAN)+"*********************************************"+RESET);
        printCentered(std::string(CYAN)+"*              [ Please Login ]             *"+RESET);
        printCentered(std::string(CYAN)+"*********************************************"+RESET);
        std::cout << "\n";

        int padInput = (getTerminalWidth()-37)/2;
        std::string pi(padInput,' ');

        std::cout << pi << "Username : " << GREEN << std::flush;
        username = getInputSameLine(); 
        std::cout << RESET;

        std::cout << pi << "Password : " << GREEN << std::flush;
        password = getPasswordInput(); 
        std::cout << RESET;

        if (login(username, password)) {
            std::cout << "\n";
            int padMsg = (getTerminalWidth()-30)/2;
            std::cout << std::string(padMsg,' ') << GREEN << "Login successful! Welcome, "
                      << getCurrentUsername() << "!" << RESET << "\n";
            pressEnterToContinue(); 
            return;
        } else {
            int padMsg = (getTerminalWidth()-30)/2;
            std::cout << "\n" << std::string(padMsg,' ') << RED << "Invalid username or password!" << RESET << "\n";
            pressEnterToContinue();
        }
    }
}

void Manager::signUp() {
    clearScreen();
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*             [ Please SignUp ]             *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";

    std::string name,gender,position,phone,email,username,password;
    int age=0; double salary=0.0;
    int pad=(getTerminalWidth()-38)/2;
    std::string p(pad,' ');
    std::cout << "\n" << p << "Create Your Account\n\n";
    std::cout << p << "Name     : " << GREEN << std::flush; 
    name=getInputSameLine(); 
    std::cout << RESET;

    while(true){
        std::cout << p << "Age      : " << GREEN << std::flush;
        std::string s=getInputSameLine(); 
        std::cout << RESET;
        try{age=std::stoi(s);break;}catch(...){std::cout<<p<<RED<<"Invalid age!\n"<<RESET;}
    }

    std::cout << p << "Gender   : " << GREEN << std::flush; 
    gender=getInputSameLine(); 
    std::cout << RESET;

    std::cout << p << "Position : " << GREEN << std::flush; 
    position=getInputSameLine(); 
    std::cout << RESET;

    while(true){
        std::cout << p << "Salary   : " << GREEN << std::flush;
        std::string s=getInputSameLine(); 
        std::cout << RESET;
        try{salary=std::stod(s);break;}catch(...){std::cout<<p<<RED<<"Invalid salary!\n"<<RESET;}
    }

    std::cout << p << "Phone    : " << GREEN << std::flush; 
    phone=getInputSameLine(); 
    std::cout << RESET;

    std::cout << p << "Email    : " << GREEN << std::flush; 
    email=getInputSameLine(); 
    std::cout << RESET;

    std::cout << "\n" << p << "Create Login Credentials:\n\n";

    while(true){
        std::cout << p << "Username : " << GREEN << std::flush; username=getInputSameLine(); std::cout << RESET;
        if(usernameExists(username)){std::cout<<p<<RED<<"Username taken!\n"<<RESET;}else break;
    }
    std::cout << p << "Password : " << GREEN << std::flush; 
    password=getInputSameLine(); 
    std::cout << RESET;

    std::string newId=generateNextId();
    Employee emp(newId,name,age,gender,position,salary,phone,email,username,password,"employee");
    employees.push_back(emp); saveEmployees();
    std::string msg="Account created! ID: "+newId+" | Role: employee";
    int padMsg=(getTerminalWidth()-(int)msg.size())/2;
    std::cout << "\n" << std::string(padMsg,' ') << GREEN << msg << RESET << "\n";
    pressEnterToContinue();
}

bool Manager::login(const std::string& username, const std::string& password) {
    if (hardcodedAdmin.matches(username, password)) { currentUser=&hardcodedAdmin; return true; }
    for (Employee& emp : employees) {
        if (emp.matches(username, password)) { currentUser=&emp; return true; }
    }
    return false;
}

void Manager::logout() { currentUser=nullptr; }
bool Manager::isLoggedIn() const { return currentUser!=nullptr; }
bool Manager::currentUserIsAdmin() const { return currentUser!=nullptr && currentUser->isAdmin(); }
std::string Manager::getCurrentUsername() const { return currentUser ? currentUser->username : ""; }

void Manager::addEmployee() {
    clearScreen(); 
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*            [ Add New Employee ]           *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";

    std::string name,gender,position,phone,email,username,password,role;
    int age=0; double salary=0.0;

    int pad=(getTerminalWidth()-38)/2; 
    std::string p(pad,' ');

    std::cout << "\n" << p << "Enter Employee Details:\n\n";

    std::cout<<p<<"Name     : "<<GREEN<<std::flush; 
    name=getInputSameLine(); 
    std::cout<<RESET;

    while(true){ std::cout<<p<<"Age      : "<<GREEN<<std::flush; std::string s=getInputSameLine(); std::cout<<RESET;
        try{age=std::stoi(s);break;}catch(...){std::cout<<p<<RED<<"Invalid age!\n"<<RESET;} }

    std::cout<<p<<"Gender   : "<<GREEN<<std::flush; 
    gender=getInputSameLine(); 
    std::cout<<RESET;

    std::cout<<p<<"Position : "<<GREEN<<std::flush; 
    position=getInputSameLine(); 
    std::cout<<RESET;

    while(true){ std::cout<<p<<"Salary   : "<<GREEN<<std::flush; std::string s=getInputSameLine(); std::cout<<RESET;
        try{salary=std::stod(s);break;}catch(...){std::cout<<p<<RED<<"Invalid salary!\n"<<RESET;} }

    std::cout<<p<<"Phone    : "<<GREEN<<std::flush; 
    phone=getInputSameLine(); 
    std::cout<<RESET;

    std::cout<<p<<"Email    : "<<GREEN<<std::flush; 
    email=getInputSameLine(); 
    std::cout<<RESET;

    std::cout<<"\n"<<p<<"Enter Login Credentials:\n\n";

    while(true){ std::cout<<p<<"Username : "<<GREEN<<std::flush; 
        username=getInputSameLine(); 
        std::cout<<RESET;
        if(usernameExists(username)){std::cout<<p<<RED<<"Username taken!\n"<<RESET;}else break; }

    std::cout<<p<<"Password : "<<GREEN<<std::flush; 
    password=getInputSameLine(); 
    std::cout<<RESET;

    while(true){ std::cout<<p<<"Role (admin/employee) : "<<GREEN<<std::flush; 
        role=getInputSameLine(); 
        std::cout<<RESET;

        if(role=="admin"||role=="employee")break;
        std::cout<<p<<RED<<"Invalid! Enter 'admin' or 'employee'.\n"<<RESET; }
    std::string newId=generateNextId();
    employees.push_back(Employee(newId,name,age,gender,position,salary,phone,email,username,password,role));
    saveEmployees();

    std::string msg="Employee added! ID: "+newId;
    int padMsg=(getTerminalWidth()-(int)msg.size())/2;
    std::cout<<"\n"<<std::string(padMsg,' ')<<GREEN<<msg<<RESET<<"\n";
    pressEnterToContinue();
}

void Manager::viewEmployee() {
    int choice;
    while(true){
        clearScreen(); 
        printHeader("View Employee");
        printMenuItem("[1]","View All"); 
        printMenuItem("[2]","Sort by ID");
        printMenuItem("[3]","Sort by Salary"); 
        printMenuItem("[0]","Back"); 
        printFooter();

        std::string choiceStr; 
        int pad=(getTerminalWidth()-38)/2;
        std::cout<<"\n"<<std::string(pad,' ')<<"Enter choice: "<<GREEN<<std::flush;
        choiceStr=getInputSameLine(); 
        std::cout<<RESET;

        try{choice=std::stoi(choiceStr);}catch(...){choice=-1;}
        if(choice==1){ displayTablePaginated(employees); }

        else if(choice==2){
            std::vector<Employee> sorted=employees;
            std::sort(sorted.begin(),sorted.end(),[](const Employee&a,const Employee&b){return a.id<b.id;});
            displayTablePaginated(sorted);
        } else if(choice==3){
            std::vector<Employee> sorted=employees;
            std::sort(sorted.begin(),sorted.end(),[](const Employee&a,const Employee&b){return a.salary>b.salary;});
            displayTablePaginated(sorted);
        } else if(choice==0) break;
        else { int padMsg=(getTerminalWidth()-22)/2; std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"Invalid choice!"<<RESET<<"\n"; 
            pressEnterToContinue(); 
        }
    }
}

void Manager::editEmployee() {
    clearScreen(); 
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*             [ Edit Employee ]             *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";

    int pad=(getTerminalWidth()-38)/2; 
    std::string p(pad,' ');

    std::cout<<"\n"<<p<<"Enter Employee ID to edit: "<<GREEN<<std::flush;
    std::string searchId=getInputSameLine(); 
    std::cout<<RESET;

    for(Employee& emp:employees){
        if(emp.hasId(searchId)){
            emp.displayDetails(true);
            std::cout<<"\n"<<p<<"Press Enter to keep current value.\n\n";

            std::cout<<p<<"New Name ["<<emp.name<<"] : "<<GREEN<<std::flush;
            std::string newName=getInputOptional(); 
            std::cout<<RESET;

            if(!newName.empty()) emp.name=newName;
            std::cout<<p<<"New Position ["<<emp.position<<"] : "<<GREEN<<std::flush;
            std::string newPos=getInputOptional(); 
            std::cout<<RESET;

            if(!newPos.empty()) emp.position=newPos;
            std::cout<<p<<"New Salary ["<<emp.salary<<"] : "<<GREEN<<std::flush;
            std::string newSal=getInputOptional(); 
            std::cout<<RESET;

            if(!newSal.empty()){try{emp.salary=std::stod(newSal);}catch(...){std::cout<<p<<RED<<"Invalid salary.\n"<<RESET;}}
            saveEmployees();

            int padMsg=(getTerminalWidth()-30)/2;
            std::cout<<"\n"<<std::string(padMsg,' ')<<GREEN<<"Employee updated successfully!"<<RESET<<"\n";
            pressEnterToContinue(); 
            return;
        }
    }
    int padMsg=(getTerminalWidth()-30)/2;
    std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"ID '"<<searchId<<"' not found."<<RESET<<"\n";
    pressEnterToContinue();
}

void Manager::deleteEmployee() {
    clearScreen(); 
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*             [ Delete Employee ]           *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";
    
    int pad=(getTerminalWidth()-38)/2; 
    std::string p(pad,' ');

    std::cout<<"\n"<<p<<"Enter Employee ID to delete: "<<GREEN<<std::flush;
    std::string searchId=getInputSameLine(); 
    std::cout<<RESET;

    if(currentUser&&currentUser->hasId(searchId)){
        int padMsg=(getTerminalWidth()-36)/2;
        std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"Cannot delete your own account!"<<RESET<<"\n";
        pressEnterToContinue(); 
        return;
    }
    for(int i=0;i<(int)employees.size();i++){
        if(employees[i].hasId(searchId)){
            employees[i].displayDetails(true);

            std::cout << "\n";
            std::cout << p << CYAN << "+------------------------------------+\n" << RESET;
            std::cout << p << CYAN << "|" << RESET << RED  << "          WARNING                   " << RESET << CYAN << "|\n" << RESET;
            std::cout << p << CYAN << "+------------------------------------+\n" << RESET;
            std::cout << p << CYAN << "|" << RESET << "                                    " << CYAN << "|\n" << RESET;
            std::cout << p << CYAN << "|" << RESET << WHITE << "  Delete: " << std::left << std::setw(26) << employees[i].name << RESET << CYAN << "|\n" << RESET;
            std::cout << p << CYAN << "|" << RESET << RED   << "  This action cannot be undone!     " << RESET << CYAN << "|\n" << RESET;
            std::cout << p << CYAN << "|" << RESET << "                                    " << CYAN << "|\n" << RESET;
            std::cout << p << CYAN << "|" << RESET << GREEN << "  [Y] Confirm    " << RED << "  [N] Cancel       " << RESET << CYAN << "|\n" << RESET;
            std::cout << p << CYAN << "+------------------------------------+\n" << RESET;
            std::cout << "\n" << p << "Enter choice: " << GREEN << std::flush;
            std::string confirm = getInputSameLine(); std::cout << RESET;

            if(confirm=="y"||confirm=="Y"){
                employees.erase(employees.begin()+i); saveEmployees();
                int padMsg=(getTerminalWidth()-30)/2;
                std::cout<<"\n"<<std::string(padMsg,' ')<<GREEN<<"Employee deleted successfully!"<<RESET<<"\n";
            } else {
                int padMsg=(getTerminalWidth()-20)/2;
                std::cout<<"\n"<<std::string(padMsg,' ')<<YELLOW<<"Deletion cancelled."<<RESET<<"\n";
            }
            pressEnterToContinue(); 
            return;
        }
    }
    int padMsg=(getTerminalWidth()-30)/2;
    std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"ID '"<<searchId<<"' not found."<<RESET<<"\n";
    pressEnterToContinue();
}

void Manager::manageAttendance() {
    clearScreen(); 
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*          [ Attendance & Payment ]         *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";

    int pad=(getTerminalWidth()-38)/2; 
    std::string p(pad,' ');

    std::cout<<"\n"<<p<<"Enter Employee ID: "<<GREEN<<std::flush;
    std::string searchId=getInputSameLine(); 
    std::cout<<RESET;

    for(const Employee& emp:employees){
        if(emp.hasId(searchId)){
            emp.displayDetails(false);
            int absentDays=0;

            while(true){
                std::cout<<"\n"<<p<<"Enter absent days this month: "<<GREEN<<std::flush;
                std::string daysStr=getInputSameLine(); 
                std::cout<<RESET;

                try{absentDays=std::stoi(daysStr); if(absentDays<0)throw std::invalid_argument(""); break;}
                catch(...){std::cout<<p<<RED<<"Invalid! Enter a positive number.\n"<<RESET;}
            }
            Attendance att(emp.id,emp.name,getCurrentMonth(),absentDays,emp.salary);
            att.displayPayment();
            attendances.push_back(att); 
            saveAttendances();
            int padMsg=(getTerminalWidth()-30)/2;
            std::cout<<"\n"<<std::string(padMsg,' ')<<GREEN<<"Attendance recorded!"<<RESET<<"\n";
            pressEnterToContinue(); 
            return;
        }
    }
    int padMsg=(getTerminalWidth()-30)/2;
    std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"ID '"<<searchId<<"' not found."<<RESET<<"\n";
    pressEnterToContinue();
}

void Manager::viewReports() {
    clearScreen(); 
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*               [ View Report ]             *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";

    int pad=(getTerminalWidth()-38)/2; 
    std::string p(pad,' ');

    if(reports.empty()){
        int padMsg=(getTerminalWidth()-20)/2;
        std::cout<<"\n"<<std::string(padMsg,' ')<<YELLOW<<"No reports found."<<RESET<<"\n";
        pressEnterToContinue(); 
        return;
    }
    std::cout<<"\n"<<p<<YELLOW<<"Total Reports: "<<reports.size()<<RESET<<"\n\n";
    for(const Report& r:reports) r.displayReport();
    pressEnterToContinue();
}

void Manager::backupManagement() {
    int choice;
    while(true){
        clearScreen(); 
        printHeader("Backup Management");
        printMenuItem("[1]","View Backups"); 
        printMenuItem("[2]","Restore Backup"); 
        printMenuItem("[0]","Back"); 
        printFooter();

        int pad=(getTerminalWidth()-38)/2;
        std::string choiceStr;
        std::cout<<"\n"<<std::string(pad,' ')<<"Enter choice: "<<GREEN<<std::flush;
        choiceStr=getInputSameLine(); 
        std::cout<<RESET;

        try{choice=std::stoi(choiceStr);}catch(...){choice=-1;}
        if(choice==1) viewBackups();
        else if(choice==2) restoreBackup();
        else if(choice==0) 
        break;
        else{int padMsg=(getTerminalWidth()-22)/2; 
            std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"Invalid choice!"<<RESET<<"\n"; 
            pressEnterToContinue();
        }
    }
}

void Manager::searchEmployee() {
    int choice;
    while(true){
        clearScreen(); 
        printHeader("Search Employee");
        printMenuItem("[1]","Search by ID"); 
        printMenuItem("[2]","Search by Name"); 
        printMenuItem("[0]","Back"); 
        printFooter();

        int pad=(getTerminalWidth()-38)/2; 
        std::string p(pad,' ');

        std::string choiceStr; 
        std::cout<<"\n"<<p<<"Enter choice: "<<GREEN<<std::flush; 
        choiceStr=getInputSameLine(); 
        std::cout<<RESET;

        try{choice=std::stoi(choiceStr);}catch(...){choice=-1;}
        if(choice==1){
            clearScreen(); 
            printCentered(std::string(CYAN)+"*********************************************"+RESET);
            printCentered(std::string(CYAN)+"*              [ Search by ID ]             *"+RESET);
            printCentered(std::string(CYAN)+"*********************************************"+RESET);
            std::cout << "\n";

            std::cout<<"\n"<<p<<"Enter Employee ID: "<<GREEN<<std::flush;
            std::string searchId=getInputSameLine(); 
            std::cout<<RESET;

            bool found=false;
            for(const Employee& emp:employees){
                if(emp.hasId(searchId)){emp.displayDetails(currentUserIsAdmin()); 
                    found=true; 
                    break;
                }
            }
            if(!found){int padMsg=(getTerminalWidth()-30)/2; std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"No employee found with ID '"<<searchId<<"'."<<RESET<<"\n";}
            pressEnterToContinue();
        } else if(choice==2){
            clearScreen(); 
            printCentered(std::string(CYAN)+"*********************************************"+RESET);
            printCentered(std::string(CYAN)+"*             [ Search by Name ]            *"+RESET);
            printCentered(std::string(CYAN)+"*********************************************"+RESET);
            std::cout << "\n";

            std::cout<<"\n"<<p<<"Enter name to search: "<<GREEN<<std::flush;
            std::string searchTerm=getInputSameLine(); 
            std::cout<<RESET;

            std::vector<Employee> 
            results;
            for(const Employee& emp:employees) if(emp.nameContains(searchTerm)) results.push_back(emp);
            if(results.empty()){
                int padMsg=(getTerminalWidth()-40)/2;
                std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"No employee found with name containing '"<<searchTerm<<"'."<<RESET<<"\n";
                pressEnterToContinue();
            } else {
                int padMsg=(getTerminalWidth()-20)/2;
                std::cout<<"\n"<<std::string(padMsg,' ')<<GREEN<<"Found "<<results.size()<<" result(s):"<<RESET<<"\n";
                displayTablePaginated(results);
            }
        } else if(choice==0) break;
        else{int padMsg=(getTerminalWidth()-22)/2; std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"Invalid choice!"<<RESET<<"\n"; 
            pressEnterToContinue();
        }
    }
}

void Manager::viewMyProfile() {
    clearScreen(); 
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*               [ My Profile ]             *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";

    for(const Employee& emp:employees){
        if(emp.username==currentUser->username){emp.displayDetails(true); 
            pressEnterToContinue(); 
            return;
        }
    }
    int padMsg=(getTerminalWidth()-20)/2;
    std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"Profile not found."<<RESET<<"\n";
    pressEnterToContinue();
}

void Manager::editMyProfile() {
    clearScreen(); 
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*            [ Edite My Profile ]           *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";

    int pad=(getTerminalWidth()-38)/2; 
    std::string p(pad,' ');
    for(Employee& emp:employees){
        if(emp.username==currentUser->username){
            emp.displayDetails(true);

            std::cout<<"\n"<<p<<"Press Enter to keep current value.\n\n";

            std::cout<<p<<"New Name ["<<emp.name<<"] : "<<GREEN<<std::flush;
            std::string newName=getInputOptional(); 
            std::cout<<RESET;

            if(!newName.empty()) emp.name=newName;
            std::cout<<p<<"New Phone ["<<emp.phone<<"] : "<<GREEN<<std::flush;
            std::string newPhone=getInputOptional(); 
            std::cout<<RESET;

            if(!newPhone.empty()) emp.phone=newPhone;
            std::cout<<p<<"New Email ["<<emp.email<<"] : "<<GREEN<<std::flush;
            std::string newEmail=getInputOptional(); 
            std::cout<<RESET;

            if(!newEmail.empty()) emp.email=newEmail;
            currentUser=&emp; 
            saveEmployees();
            int padMsg=(getTerminalWidth()-26)/2;
            std::cout<<"\n"<<std::string(padMsg,' ')<<GREEN<<"Profile updated successfully!"<<RESET<<"\n";
            pressEnterToContinue(); 
            return;
        }
    }
}

void Manager::writeReport() {
    clearScreen(); 
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*              [ Write Report ]             *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";

    int pad=(getTerminalWidth()-38)/2; 
    std::string p(pad,' ');

    std::cout<<"\n"<<p<<"Write your report to Admin:\n\n";
    std::string subject,message;

    std::cout<<p<<"Subject : "<<GREEN<<std::flush; 
    subject=getInputSameLine(); 
    std::cout<<RESET;

    std::cout<<p<<"Message : "<<GREEN<<std::flush; 
    message=getInputSameLine(); 
    std::cout<<RESET;

    Report r(getCurrentUsername(),getTodayDate(),subject,message);
    reports.push_back(r); saveReports();
    int padMsg=(getTerminalWidth()-26)/2;
    std::cout<<"\n"<<std::string(padMsg,' ')<<GREEN<<"Report sent successfully!"<<RESET<<"\n";
    pressEnterToContinue();
}

void Manager::viewMyReports() {
    clearScreen(); 
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*                [ My Report ]              *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";

    int pad=(getTerminalWidth()-38)/2; std::string p(pad,' ');
    std::vector<Report> myReports;
    for(const Report& r:reports) if(r.from==getCurrentUsername()) myReports.push_back(r);

    if(myReports.empty()){
        int padMsg=(getTerminalWidth()-26)/2;
        std::cout<<"\n"<<std::string(padMsg,' ')<<YELLOW<<"You have no reports yet."<<RESET<<"\n";
        pressEnterToContinue(); 
        return;
    }
    std::cout<<"\n"<<p<<YELLOW<<"Your Reports ("<<myReports.size()<<" total):"<<RESET<<"\n";
    for(const Report& r:myReports) r.displayReport();
    pressEnterToContinue();
}

void Manager::viewMyPayment() {
    clearScreen(); 
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    printCentered(std::string(CYAN)+"*             [ Payment History ]           *"+RESET);
    printCentered(std::string(CYAN)+"*********************************************"+RESET);
    std::cout << "\n";

    int pad=(getTerminalWidth()-38)/2; 
    std::string p(pad,' ');

    std::vector<Attendance> myPayments;
    for(const Attendance& att:attendances) if(att.employeeId==currentUser->id) myPayments.push_back(att);
    if(myPayments.empty()){
        int padMsg=(getTerminalWidth()-26)/2;
        std::cout<<"\n"<<std::string(padMsg,' ')<<YELLOW<<"No payment records found."<<RESET<<"\n";
        pressEnterToContinue(); 
        return;
    }
    std::cout<<"\n"<<p<<YELLOW<<"Payment History ("<<myPayments.size()<<" records):"<<RESET<<"\n";
    for(const Attendance& att:myPayments) att.displayPayment();
    pressEnterToContinue();
}

void Manager::showAdminMenu() {
    int choice;
    while(true){
        clearScreen();
        std::string subtitle="Welcome!, "+getCurrentUsername()+" [ADMIN]";
        printHeader(subtitle);
        printMenuItem("[1]","Add Employee"); 
        printMenuItem("[2]","View Employee");
        printMenuItem("[3]","Edit Employee"); 
        printMenuItem("[4]","Delete Employee");
        printMenuItem("[5]","Search Employee"); 
        printMenuItem("[6]","Attendance & Payment");
        printMenuItem("[7]","View Reports"); 
        printMenuItem("[8]","Backup Management");
        printMenuItem("[0]","Logout"); 
        printFooter();

        int pad=(getTerminalWidth()-38)/2;
        std::string choiceStr; 
        std::cout<<"\n"<<std::string(pad,' ')<<"Enter choice: "<<GREEN<<std::flush;
        choiceStr=getInputSameLine(); 
        std::cout<<RESET;

        try{choice=std::stoi(choiceStr);}catch(...){choice=-1;}
        switch(choice){
            case 1: addEmployee(); break;
            case 2: viewEmployee(); break;
            case 3: editEmployee(); break;
            case 4: deleteEmployee(); break;
            case 5: searchEmployee(); break;
            case 6: manageAttendance(); break;
            case 7: viewReports(); break;
            case 8: backupManagement(); break;
            case 0:{
                logout(); clearScreen();
                int padMsg=(getTerminalWidth()-24)/2;
                std::cout<<"\n"<<std::string(padMsg,' ')<<GREEN<<"Logged out successfully!"<<RESET<<"\n";
                pressEnterToContinue(); 
                return;
            }
            default:{
                int padMsg=(getTerminalWidth()-22)/2;
                std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"Invalid choice!"<<RESET<<"\n";
                pressEnterToContinue();
            }
        }
    }
}

void Manager::showEmployeeMenu() {
    int choice;
    while(true){
        clearScreen();
        std::string subtitle="Welcome!, "+getCurrentUsername()+" [EMPLOYEE]";
        printHeader(subtitle);
        printMenuItem("[1]","View My Profile"); 
        printMenuItem("[2]","Edit My Profile");
        printMenuItem("[3]","Write Report"); 
        printMenuItem("[4]","View My Reports");
        printMenuItem("[5]","View Payment"); 
        printMenuItem("[0]","Logout"); 
        printFooter();

        int pad=(getTerminalWidth()-38)/2;
        std::string choiceStr; 
        std::cout<<"\n"<<std::string(pad,' ')<<"Enter choice: "<<GREEN<<std::flush;
        choiceStr=getInputSameLine(); 
        std::cout<<RESET;

        try{choice=std::stoi(choiceStr);}catch(...){choice=-1;}
        switch(choice){
            case 1: viewMyProfile(); break;
            case 2: editMyProfile(); break;
            case 3: writeReport(); break;
            case 4: viewMyReports(); break;
            case 5: viewMyPayment(); break;
            case 0:{
                logout(); 
                clearScreen();
                int padMsg=(getTerminalWidth()-24)/2;
                std::cout<<"\n"<<std::string(padMsg,' ')<<GREEN<<"Logged out successfully!"<<RESET<<"\n";
                pressEnterToContinue(); 
                return;
            }
            default:{
                int padMsg=(getTerminalWidth()-22)/2;
                std::cout<<"\n"<<std::string(padMsg,' ')<<RED<<"Invalid choice!"<<RESET<<"\n";
                pressEnterToContinue();
            }
        }
    }
}