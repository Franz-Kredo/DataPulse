# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17
CXXDEBUGFLAGS = -ggdb -fsanitize=address -fsanitize=undefined
INCLUDES = -Iinclude
LIBS = -lssh

# Production source files (all .cpp files in src/ and its subdirectories)
PROD_SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard UILayer/*.cpp) $(wildcard Models/*.cpp) $(wildcard LogicLayer/*.cpp) $(wildcard Tests/LexiTests/*.cpp)
PROD_OBJ = $(PROD_SRC:.cpp=.o)

# All source files for the main build: main.cpp plus the production sources
SRC_FILES = main.cpp $(PROD_SRC)
OBJFILES = $(SRC_FILES:.cpp=.o)

# Name of the final executable
TARGET = datapulse.out

.PHONY: all debug run short fast deb clean fresh help

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
	gdb --args ./debug.out 192.168.42.125 franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester /home/franz/.ssh/pa5

# Run the executable
run: $(TARGET)
	./$(TARGET) 192.168.42.125 franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester /home/franz/.ssh/pa5 

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
	gdb --args ./debug.out 192.168.42.125 franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester

# Pattern rule: compile any .cpp file into a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files and executables
clean:
	rm -f $(OBJFILES) $(TARGET) debug.out
	@echo "Cleaned project."

# Fresh: clean and then rebuild everything
fresh: clean all
