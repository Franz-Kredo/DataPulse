# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17
CXXDEBUGFLAGS = -ggdb -fsanitize=address -fsanitize=undefined
INCLUDES = -Iinclude
LIBS = -lssh

# Production source files (all .cpp files in src/ and its subdirectories)
PROD_SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard UILayer/*.cpp) $(wildcard Models/*.cpp) $(wildcard LogicLayer/*.cpp)
PROD_OBJ = $(PROD_SRC:.cpp=.o)

# All source files for the main build: main.cpp plus the production sources
SRC_FILES = main.cpp $(PROD_SRC)
OBJFILES = $(SRC_FILES:.cpp=.o)

# Name of the final executable
TARGET = datapulse.out

.PHONY: all debug run short fast deb clean fresh test

# Default target: build the full executable from all source files
all: $(TARGET)

# Link object files to create the main executable
$(TARGET): $(OBJFILES)
	@echo "Linking object files to create executable..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJFILES) $(LIBS)

# Debug build: clean first, compile with debug flags, then launch gdb
debug: clean
	@echo "Building debug version..."
	$(CXX) $(CXXFLAGS) $(CXXDEBUGFLAGS) $(INCLUDES) -o debug.out $(SRC_FILES) $(LIBS)
	gdb ./debug.out

# Run the executable
run: $(TARGET)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJFILES) $(LIBS)
	./$(TARGET) 


# Run tests for the is_network_command method
net:clean $(TARGET)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJFILES) $(LIBS)
	@echo "\n\n\n============================================================="
	@echo "============== Running Valid Net Command Tests =============="
	@echo "============================================================="
	@echo "\n\n====== Running Net Test 1 ======"
	./$(TARGET) 192.168.42.125 franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester
	@echo "\n\n====== Running Net Test 2 ======"
	./$(TARGET) 192.168.42.125 franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester /Users/franzisak/.ssh/proxmox-home
	@echo "\n\n====== Running Net Test 3 ======"
	./$(TARGET) 192.168.42.125 -m franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester 
	@echo "\n\n====== Running Net Test 4 ======"
	./$(TARGET) 192.168.42.125 -m franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester /Users/franzisak/.ssh/proxmox-home
	@echo "\n\n====== Running Invalid Net Test 5 (triple ///) ======"
	./$(TARGET) 192.168.42.125 franz /home/franz/pa5 /home/franz/github_repos/DataPulse///SyncTester
 
	@echo "\n\n\n==============================================================="
	@echo "============== Running Invalid Net Command Tests =============="
	@echo "==============================================================="
	@echo "\n\n====== Running Invalid Net Test 6 (missing fields, filled with spaces) ======"
	./$(TARGET) 192.168.42.125 -m franz /home/franz/pa5  
	@echo "\n\n====== Running Invalid Net Test 7 (backwards \) ======"
	./$(TARGET) 192.168.42.125 franz /home/franz/pa5 /home/franz/github_repos/DataPulse\SyncTester
	@echo "\n\n====== Running Invalid Net Test 8 (too few args) ======"
	./$(TARGET) 192.168.42.125 franz 
	@echo "\n\n====== Running Net Test 9 (incorrect argument placements) ======"
	./$(TARGET) /home/franz/github_repos/DataPulse/SyncTester 192.168.42.125 franz /home/franz/pa5 /Users/franzisak/.ssh/proxmox-home -m
	@echo "\n\n====== Running Net Test 10 (to many arguments) ======"
	./$(TARGET) 192.168.42.125 -m franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester /Users/franzisak/.ssh/proxmox-home /Users/franzisak/.ssh/proxmox-home /Users/franzisak/.ssh/proxmox-home /Users/franzisak/.ssh/proxmox-home
 

# Run tests for the is_help_command method
help:clean $(TARGET)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJFILES) $(LIBS)
	@echo "\n\n\n============================================================="
	@echo "============== Running Valid Help Command Tests ==============="
	@echo "============================================================="
	@echo "\n\n====== Running Help Test 1 ======"
	./$(TARGET) -h
	@echo "\n\n====== Running Valid Help Test 2 (long flag, filled with spaces 1) ======"
	./$(TARGET) - h
	@echo "\n\n====== Running Valid Help Test 3 (long flag, filled with spaces 2) ======"
	./$(TARGET) -h 
	@echo "\n\n====== Running Valid Help Test 4 (long flag, filled with spaces 3) ======"
	./$(TARGET)  -h
	@echo "\n\n====== Running Valid Help Test 5 (long flag, filled with spaces 4) ======"
	./$(TARGET)        -h
	@echo "\n\n\n==============================================================="
	@echo "============== Running Invalid Net Command Tests =============="
	@echo "==============================================================="
	@echo "\n\n====== Running Invalid Help Test 6 (Too many letters 1) ======"
	./$(TARGET) -hx
	@echo "\n\n====== Running Invalid Help Test 7 (Too many letters) ======"
	./$(TARGET) x-h
	@echo "\n\n====== Running Invalid Help Test 8 (Invalid flag) ======"
	./$(TARGET) -m
	


# "short" target: incremental build that compiles only changed source files
short: $(OBJFILES)
	@echo "Linking object files to create executable..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJFILES) $(LIBS)
	./$(TARGET)

# Fast: run the executable without rebuilding
fast:
	./$(TARGET) 192.168.42.125 franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester

# Deb: launch debugger on the debug build
deb:
	gdb ./debug.out

# Pattern rule: compile any .cpp file into a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# --- Test targets ---
# List test source files and corresponding test executables
TEST_SOURCES = $(wildcard tests/*.cpp)
TEST_TARGETS = $(patsubst tests/%.cpp, tests/%.out, $(TEST_SOURCES))

# 'test' target builds all test executables and runs them one by one
test: $(TEST_TARGETS)
	@echo "Running tests..."
	@for test in $(TEST_TARGETS); do \
		echo "Running $$test..."; \
		./$$test; \
	done

# Pattern rule for compiling test executables: each test file links with the production objects.
# Note that main.cpp is not used here, so we avoid duplicate main definitions.
tests/%.out: tests/%.cpp $(PROD_OBJ)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $< $(PROD_OBJ) $(LIBS)

# Clean up object files, executables, and test executables
clean:
	rm -f $(OBJFILES) $(TARGET) debug.out tests/*.out
	@echo "Cleaned project."

# Fresh: clean and then rebuild everything
fresh: clean all
