#!/bin/bash
# -------------------------------------------------------
# run.sh
# Easy build and run script for Employee Management System
# 
# HOW TO USE:
#   First time  : bash run.sh
#   After that  : bash run.sh
# -------------------------------------------------------

echo "========================================"
echo "   Employee Management System"
echo "========================================"

# Check if build folder exists
if [ ! -d "build" ]; then
    echo ""
    echo "  Build folder not found! Setting up..."
    mkdir build
    cd build
    cmake .. -G "MinGW Makefiles"
    cmake --build .
    cd ..
else
    echo ""
    echo "  Building latest changes..."
    cd build
    cmake --build .
    cd ..
fi

echo ""
echo "  Starting program..."
echo "========================================"
echo ""

# Run the program
build/EmployeeManagementSystem.exe