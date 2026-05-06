cmake_minimum_required(VERSION 3.14)
project(EmployeeManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")

add_executable(EmployeeManagementSystem
    src/main.cpp
    src/Employee.cpp
    src/Manager.cpp
)

target_include_directories(EmployeeManagementSystem PRIVATE
    include
)