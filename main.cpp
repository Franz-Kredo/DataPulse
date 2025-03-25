#include <libssh/libssh.h>
#include <iostream>

int main() {
    ssh_session session = ssh_new();
    if (!session) {
        std::cerr << "Failed to create SSH session." << std::endl;
        return 1;
    }
    std::cout << "Successfully created SSH session." << std::endl;
    ssh_free(session);
    return 0;
}
