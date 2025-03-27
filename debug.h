#ifndef DEBUGGING_H
#define DEBUGGING_H

#include <iostream>
#include <fstream>
#include <streambuf>
#include <string>
#include <unistd.h>

struct NullBuffer : public std::streambuf {
    int overflow(int c) override { return c; }
};

struct NullStream : public std::ostream {
    NullStream() : std::ostream(&m_sb) {}
private:
    NullBuffer m_sb;
};

// Function to check if GDB is attached
inline bool is_debugger_attached() {
    std::ifstream status_file("/proc/self/status");
    if (!status_file.is_open()) return false;

    std::string line;
    while (std::getline(status_file, line)) {
        if (line.find("TracerPid:") == 0) {
            int tracer_pid = std::stoi(line.substr(10));
            return tracer_pid > 0;  // If TracerPid > 0, a debugger is attached.
        }
    }
    return false;
}

// Function to disable stdout if running in gdb
inline void disable_stdout() {
    if (is_debugger_attached()) {  // Now disables output only if GDB is attached.
        static NullStream null_stream;
        std::cout.rdbuf(null_stream.rdbuf());
    }
}

// Function to enable stdout
inline void enable_stdout() {
    static std::streambuf *orig_buf = std::cout.rdbuf();
    std::cout.rdbuf(orig_buf);
}

#endif // DEBUGGING_H