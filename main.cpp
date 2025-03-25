#include <libssh/libssh.h>
#include <iostream>

using namespace std;

int main(int argc, const char * argv[]) {
    cout << argc << endl;

    // for(char item: argv){
        
    // }
    for (int i = 0; i < argc; i++) {
        cout << "Argument " << i << ": " << argv[i] << endl;
    }

    return 0;
}
