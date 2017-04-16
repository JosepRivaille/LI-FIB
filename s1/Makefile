CXX = g++ -std=c++11
CXXFLAGS = -O3 -Wshadow -Wall -Wextra -Wno-uninitialized -Wno-sign-compare

all: SAT-alumnes.cpp
	$(CXX) $(CXXFLAGS) SAT-alumnes.cpp -o sat
	@echo "Compiled "$<" successfully!"

clean:
	rm sat
	@echo "Cleanup complete!"
