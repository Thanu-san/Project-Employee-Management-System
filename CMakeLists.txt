cmake_minimum_required(VERSION 3.14)
project(EmployeeManagementSystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(FetchContent)

FetchContent_Declare(
    tabulate
    GIT_REPOSITORY https://github.com/p-ranav/tabulate.git
    GIT_TAG        master
)
FetchContent_MakeAvailable(tabulate)

add_executable(EmployeeManagementSystem
    src/main.cpp
    src/Employee.cpp
    src/User.cpp
    src/Manager.cpp
)

target_include_directories(EmployeeManagementSystem PRIVATE
    include
    ${tabulate_SOURCE_DIR}/include
)

target_link_libraries(EmployeeManagementSystem PRIVATE
    tabulate::tabulate
)