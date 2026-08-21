# -------------------------------------------------------
# bash run.sh
# Clean build and run script for Employee Management System
# -------------------------------------------------------

echo "========================================"
echo "   Employee Management System"
echo "========================================"
echo ""
echo "  Cleaning old build..."

rm -rf build
mkdir build

echo "  Setting up CMake..."
cd build
cmake .. -G "MinGW Makefiles" -Wno-dev

echo ""
echo "  Building..."
cmake --build .

cd ..

echo ""
echo "  Starting program..."
echo "========================================"
echo ""

build/EmployeeManagementSystem.exe