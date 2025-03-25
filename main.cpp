#include <libssh/libssh.h>
#include <iostream>
#include "UILayer/IOHandler.h"


using namespace std;

int main(int argc, const char * argv[]) {

    IOHandler::output_title("test");
    // for(char item: argv){
        
    // }
    for (int i = 0; i < argc; i++) {
        cout << "Argument " << i << ": " << argv[i] << endl;
    }

    return 0;
}
