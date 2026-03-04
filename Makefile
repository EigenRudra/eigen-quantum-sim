# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O3

# IMPORTANT: Change this path to wherever Eigen is installed on your system!
# EG: Common Linux path is /usr/include/eigen3
EIGEN_PATH = /usr/include/eigen3 

# Build target
TARGET = quantum_sim

all: $(TARGET)

$(TARGET): main.cpp
	$(CXX) $(CXXFLAGS) -I $(EIGEN_PATH) main.cpp -o $(TARGET)

clean:
	rm -f $(TARGET)
