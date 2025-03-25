# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17
CXXDEBUGFLAGS = -ggdb -fsanitize=address -fsanitize=undefined
INCLUDES = -Iinclude
LIBS = -lssh

# Production source files (all .cpp files in src/ and its subdirectories)
PROD_SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp)
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
	./$(TARGET)

# "short" target: incremental build that compiles only changed source files
short: $(OBJFILES)
	@echo "Linking object files to create executable..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJFILES) $(LIBS)
	./$(TARGET)

# Fast: run the executable without rebuilding
fast:
	./$(TARGET)

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
