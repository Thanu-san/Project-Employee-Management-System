#include "../include/Manager.hpp"
#include <iostream>
#include <stdexcept>
#include <locale>

int main() {
    try {
        std::locale::global(std::locale("C"));
    } catch (...) {}

    try {

        Manager manager("data/employees.csv");
        std::cout << "Manager created successfully!" << std::endl;

        while (true) {
            manager.showLoginScreen();

            if (manager.currentUserIsAdmin()) {
                manager.showAdminMenu();
            } else {
                manager.showEmployeeMenu();
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    } catch (...) {
        std::cerr << "UNKNOWN ERROR occurred!" << std::endl;
        std::cout << "Press Enter to exit...";
        std::cin.get();
        return 1;
    }

    return 0;
}