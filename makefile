# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++17
CXXDEBUGFLAGS = -ggdb -fsanitize=address -fsanitize=undefined
INCLUDES = -Iinclude
LIBS = -lssh -lcrypto

# Production source files (all .cpp files in src/ and its subdirectories)
PROD_SRC = $(wildcard src/*.cpp) $(wildcard src/*/*.cpp) $(wildcard UILayer/*.cpp) $(wildcard Models/*.cpp) $(wildcard LogicLayer/*.cpp) $(wildcard Tests/LexiTests/*.cpp)
PROD_OBJ = $(PROD_SRC:.cpp=.o)

# All source files for the main build: main.cpp plus the production sources
SRC_FILES = main.cpp $(PROD_SRC)
OBJFILES = $(SRC_FILES:.cpp=.o)

# Name of the final executable
TARGET = datap

.PHONY: all debug run short fast deb clean fresh help

# Default target: build the full executable from all source files
all: $(TARGET)

# Link object files to create the main executable
$(TARGET):clean $(OBJFILES)
	@echo "Linking object files to create executable..."
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJFILES) $(LIBS)

# # Debug build: clean first, compile with debug flags, then launch gdb
# debug: clean
# 	@echo "Building debug version..."
# 	$(CXX) $(CXXFLAGS) $(CXXDEBUGFLAGS) $(INCLUDES) -o debug.out $(SRC_FILES) $(LIBS)
# 	gdb --args ./debug.out 192.168.42.125 franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester /home/franz/.ssh/pa5

# # Run the executable
# run: $(TARGET)
# 	./$(TARGET) 192.168.42.125 franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester /home/franz/.ssh/pa5 

# # Run the executable
# merge: $(TARGET)
# 	./$(TARGET) -m 192.168.42.125 franz /home/franz/pa5 /home/franz/github_repos/DataPulse/SyncTester /home/franz/.ssh/pa5 

# # Run tests for the is_help_command method
# help: $(TARGET)
# 	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJFILES) $(LIBS)
# 	@echo "\n\n====== Running Help ======"
# 	./$(TARGET) -h

 
# Pattern rule: compile any .cpp file into a .o file
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files and executables
clean:
	rm -f $(OBJFILES) $(TARGET) debug.out
	@echo "Cleaned project."

# Fresh: clean and then rebuild everything
fresh: clean all
