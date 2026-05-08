# -------------------------------------------------------
# If you guys want to run our project, just go to Gitbash and then run like this below : 
# bash run.sh
 -------------------------------------------------------

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
