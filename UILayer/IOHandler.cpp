#include "IOHandler.h"
#include <limits>
#include <cstdlib>
#include <chrono>
#include <thread>



// Define static members
const string IOHandler::reset_code = "\033[0m";
const unordered_map<string, string> IOHandler::neon_colors = {
    {"orange", "\033[38;2;255;165;0m"},
    {"pink",   "\033[38;2;255;105;180m"},
    {"yellow", "\033[38;2;255;255;0m"},
    {"green",  "\033[38;2;0;255;0m"},
    {"blue", "\033[38;2;0;191;255m"},
    {"white",  "\033[38;2;255;255;255m"}
};
unordered_map<char, string> IOHandler::neon_color_map = {
    {'|', IOHandler::neon_colors.at("pink")},
    {'-', IOHandler::neon_colors.at("pink")},
    {'=', IOHandler::neon_colors.at("orange")},
    {'*', IOHandler::neon_colors.at("blue")}
};

int IOHandler::title_max_len = 70;
int IOHandler::subtitle_max_len = 50;
int IOHandler::msg_padding_len = 5;




//============= DATAPULE SPECIFIC METHODS =============//

bool IOHandler::file_exists(string &filepath) {
    struct stat sb;

    // Calls the function with path as argument
    // If the file/directory exists at the path, stat returns 0.
    if (stat(filepath.c_str(), &sb) == 0) {
        return true;
    } else {
        cout << "The path is invalid!" << endl;
        return false;
    }
}

bool IOHandler::validate_unix_filepath(string &filepath){
    // Credits: Cjxcz Odjcayrwl on StackOverflow
    // Link: https://stackoverflow.com/questions/537772/what-is-the-most-correct-regular-expression-for-a-unix-file-path
    regex linux_path_regex("^(?:/|/[^/\\\\]+(?:/[^/\\\\]+)*/?)$");


    return regex_match(filepath, linux_path_regex);;
}

//----- Command Line Argument Handler -----//
bool IOHandler::is_valid(int argc, const char * argv[]){
    // bool valid_net_command = IOHandler::is_network_command(argc, argv);
    // bool valid_help_command = IOHandler::is_help_command(argc, argv);

    // return (valid_net_command || valid_help_command);
    return false;
}  


CommandModel *IOHandler::is_network_command(int argc, const char * argv[]){
    
    //datap <host> <username> <remote_path> <local_path>
    //datap <host> <username> <remote_path> <local_path> <priv_key_path>
    //datap <host> <-flag> <username> <remote_path> <local_path>
    //datap <host> <-flag> <username> <remote_path> <local_path> <priv_key_path>
    
    int min_args = 5;
    int max_args = 7; // With -m flag (merge) + priv_key_path
    int offset = 0;
    
    string host = "";
    string username = "";
    string remote_path = "";
    string local_path = "";
    string priv_key_path = "";
    
    string flag = "";

    
    
    // Invalid amount of arguments
    if (argc < min_args || max_args < argc) return nullptr;
    
    // If there is more than minimum, then check if the 
    if (string(argv[2]) == "-m"){
        flag = argv[2];
        offset = 1;
    } 
    // I know, it's a little reduntant
    // I'm simply checking if there are too many argument, if there is no flag, then there should be max_args-1 amount or less
    else if(argc >= max_args && string(argv[2]) != "-m"){
        return nullptr;
    }
        
    host = argv[1]; // No offset since it's the first element
    username = argv[2+offset];
    remote_path = argv[3+offset];
    if (4+offset < argc)
        local_path = argv[4+offset];
    

    if(argc == max_args || (offset == 0 && 5 < argc) ){
        priv_key_path = argv[5+offset];
    }

    // cout << endl << "------------- Let's see what args we have -------------" << endl << endl;

    // cout << "\t==> Total args (argx): " << argc << endl;
    // cout << "\thost: " << host << endl;
    // cout << "\tflag: " << flag << endl;
    // cout << "\tusername: " << username << endl;
    // cout << "\tremote_path: " << remote_path << endl;
    // cout << "\tlocal_path: " << local_path << endl;
    // cout << "\tpriv_key_path: " << priv_key_path << endl;
    // cout << endl << "-------------------------------------------------------" << endl;


    // // Validate that the file exists
    if (!IOHandler::file_exists(local_path)) {
        cout << "Local directory `" << local_path << "` does not exist" << endl;
        // private key file does not exist
        return nullptr;
    }
    
    if (!IOHandler::validate_unix_filepath(remote_path)) {
        cout << "Remote directory `" << remote_path << "` does not fulfill our Regex" << endl;
        return nullptr;
    }
    
    CommandModel *retCommandModel = new CommandModel;


    retCommandModel->set_host(host);
    retCommandModel->set_username(username);
    retCommandModel->set_remote_path(remote_path);
    retCommandModel->set_local_path(local_path);
    retCommandModel->set_priv_key_path(priv_key_path);


    return retCommandModel;
}


bool IOHandler::is_help_command(int argc, const char * argv[]){
    int args_amount = 2;
    string flag;

    // Invalid amount of arguments
    if(argc != args_amount) return false;

    flag = argv[1];

    // If the second parameter is not '-h', then it's not really a help command is it?
    if(flag != "-h") return false;


    return true;
}


void IOHandler::display_help_page() {
    IOHandler::clear_terminal();
    IOHandler::output_subtitle("DataPulse Help Page", "pink");
    const std::string helpText = R"(
Usage:
  datapulse [options] <host> <username> <remote_path> <local_path> [priv_key_path]

Options:
  -h              Display this help message and exit.
  -m              Enable merge mode (if applicable).
  -t              [pending] Tests the ssh/sftp connection.

Usage with options:
  datapulse -h
  datapulse -m 192.168.50.42 remote_username /home/remote_username/someFolder /home/localuser/someOtherFolder /home/localuser/.ssh/priv_key
  datapulse -t 192.168.50.42 remote_username /home/remote_username/someFolder /home/localuser/someOtherFolder /home/localuser/.ssh/priv_key

Arguments:
  <host>         The remote host address (IP or domain name).
  <username>     The username for the remote connection.
  <remote_path>  The absolute path to the remote directory (must be a valid UNIX filepath).
  <local_path>   The local directory path (must exist on your system).
  [priv_key_path] (Optional) The file path to your SSH private key for authentication.

Description:
  DataPulse is a command‑line application designed to synchronize files between a
  local directory and a remote directory over SFTP. It establishes an SSH connection
  using the provided credentials, reads files from both locations, marks files that
  are eligible for synchronization, and writes updates back to both locations.

Examples:
  Basic synchronization:
    datapulse 192.168.1.100 user /remote/directory /local/directory

  Using a private key for SSH authentication:
    datapulse 192.168.1.100 user /remote/directory /local/directory /path/to/private_key

  With merge mode enabled:
    datapulse 192.168.1.100 -m user /remote/directory /local/directory

Build Instructions:
  - Ensure g++ with C++17 support is installed.
  - Use the provided makefile:
      make       : Build the executable.
      make debug : Build the debug version.

Troubleshooting:
  - Verify that the local directory exists.
  - Ensure the remote path follows UNIX filepath conventions.
  - Check your SSH credentials and private key file if authentication fails.

For further assistance, refer to the project documentation or contact the development team.
    )";

    std::cout << helpText;
}

// void IOHandler::display_help_page() {


//     cout << "\nDataPulse Help Page\n";
//     cout << "===================\n\n";

//     cout << "Usage:\n";
//     cout << "  datapulse [options] <host> <username> <remote_path> <local_path> [priv_key_path]\n\n";
    
//     cout << "Options:\n";
//     cout << "  -h              Display this help message and exit.\n";
//     cout << "  -m              Enable merge mode (if applicable).\n\n";
//     cout << "  -t              [pending] Tests the ssh/sftp connection.\n";


//     cout << "Usage with options:\n";
//     cout << "  datapulse -h\n";
//     cout << "  datapulse -m 192.168.50.42 remote_username /home//someFolder /home/localuser/someOtherFolder /home/localuser/.ssh/priv_key\n\n";
//     cout << "  datapulse -t 192.168.50.42 remote_username /home//someFolder /home/localuser/someOtherFolder /home/localuser/.ssh/priv_key\n\n";

//     cout << "Arguments:\n";
//     cout << "  <host>         The remote host address (IP or domain name).\n";
//     cout << "  <username>     The username for the remote connection.\n";
//     cout << "  <remote_path>  The absolute path to the remote directory (must be a valid UNIX filepath).\n";
//     cout << "  <local_path>   The local directory path (must exist on your system).\n";
//     cout << "  [priv_key_path] (Optional) The file path to your SSH private key for authentication.\n\n";

//     cout << "Description:\n";
//     cout << "  DataPulse is a command‑line application designed to synchronize files between a\n";
//     cout << "  local directory and a remote directory over SFTP. It establishes an SSH connection\n";
//     cout << "  using the provided credentials, reads files from both locations, marks files that\n";
//     cout << "  are eligible for synchronization, and writes updates back to both locations.\n\n";

//     cout << "Examples:\n";
//     cout << "  Basic synchronization:\n";
//     cout << "    datapulse 192.168.1.100 user /remote/directory /local/directory\n\n";

//     cout << "  Using a private key for SSH authentication:\n";
//     cout << "    datapulse 192.168.1.100 user /remote/directory /local/directory /path/to/private_key\n\n";

//     cout << "  With merge mode enabled:\n";
//     cout << "    datapulse 192.168.1.100 -m user /remote/directory /local/directory\n\n";

//     cout << "Build Instructions:\n";
//     cout << "  - Ensure g++ with C++17 support is installed.\n";
//     cout << "  - Use the provided makefile:\n";
//     cout << "      make       : Build the executable.\n";
//     cout << "      make debug : Build the debug version.\n\n";

//     cout << "Troubleshooting:\n";
//     cout << "  - Verify that the local directory exists.\n";
//     cout << "  - Ensure the remote path follows UNIX filepath conventions.\n";
//     cout << "  - Check your SSH credentials and private key file if authentication fails.\n\n";

//     cout << "For further assistance, refer to the project documentation or contact the development team.\n\n";
// }

//=====================================================//






// OUTPUT METHODS
    
//--- NEW METHOD - WITH SCROLL ---//
void IOHandler::clear_terminal() {
    // Move cursor to top
    cout << "\033[H";
    
    // Fill the screen with blank lines
    const int lines_to_print = 100;
    for (int i = 0; i < lines_to_print; ++i) {
        cout << "\n";
    }

    // Move cursor back to top
    cout << "\033[H";
}

void IOHandler::wait(unsigned int seconds) {
    this_thread::sleep_for(chrono::seconds(seconds));
}

void IOHandler::output_title(const string &title, string color) {
    // IOHandler::clear_terminal();

    // IOHandler::clear_terminal(); // Maybe add this back later, talk to team (Franz)
    vector<string> content;

    content.push_back(" ");
    content.push_back(title);
    content.push_back(" ");


    IOHandler::display_box_layout("", content, color);
}

void IOHandler::output_subtitle(const string &subtitle, string color) {
    vector<string> content;
    content.push_back(subtitle);

    IOHandler::display_box_layout("", content, color, "white", 50);

}
// void IOHandler::write_dialoge(string dialog){
//      for (const char c : dialog) {
//         cout << c << flush;
//         this_thread::sleep_for(chrono::milliseconds(10));
//     }
// }

// void IOHandler::write_story(string dialog){
//      for (const char c : dialog) {
//         cout << c << flush;
//         this_thread::sleep_for(chrono::milliseconds(5));
//     }
// }

void IOHandler::output_msg(const string &msg) {
    string msg_padded = " " + msg + " ";
    string symbols(msg_padding_len, '-');
    cout << symbols + msg_padded + symbols << "\n";
}

void IOHandler::output_options(const string &options_title, const vector<string> &options_list) {
    IOHandler::output_subtitle(options_title);
    for (size_t i = 0; i < options_list.size(); i++) {
        cout << "[" << (i + 1) << "] " << options_list[i] << "\n";
    }
    cout << "\n";
}

void IOHandler::display_box_layout(const string &title, const vector<string> &output, const string &outline_color, const string &content_color, int size) {
    const int inner_width = size; // Fixed inner width.
    string left_filler, right_filler, bottom_filler, mid_filler, top_border;
    string display_title = title;
    if ((int)display_title.size() > inner_width - 2)
        display_title = display_title.substr(0, inner_width - 2);
        
    int title_len = display_title.size();
    int total_filler = inner_width - (title_len + 2);
    int left_filler_count = total_filler / 2;
    int right_filler_count = total_filler - left_filler_count;
    
    for (int i = 0; i < left_filler_count; ++i)
        left_filler += u8"─";
    for (int i = 0; i < right_filler_count; ++i)
        right_filler += u8"─";
    for (int i = 0; i < inner_width; ++i)
        bottom_filler += u8"─";
    
    if(title != ""){
        top_border = colorize_box(u8"┌" + left_filler + " " + display_title + " " + right_filler + u8"┐", outline_color) + "\n";
        (void) mid_filler;
    } else {
        for (int i = 0; i < (total_filler - left_filler_count - right_filler_count + 2); ++i)
            mid_filler += u8"─";
        
        top_border = colorize_box(u8"┌" + left_filler + mid_filler + right_filler + u8"┐", outline_color) + "\n";
    }
    string content;
    for (const auto &line : output) {
        string adjusted_line = line;
        if ((int)adjusted_line.size() > inner_width)
            adjusted_line = adjusted_line.substr(0, inner_width);
        int pad_total = inner_width - adjusted_line.size();
        int pad_left = pad_total / 2;
        int pad_right = pad_total - pad_left;
        content += colorize_box(u8"│", outline_color) +
                   colorize_box(string(pad_left, ' ') + adjusted_line + string(pad_right, ' '), content_color) +
                   colorize_box(u8"│\n", outline_color);
    }
    string bottom_border = colorize_box(u8"└" + bottom_filler + u8"┘\n", outline_color);
    cout << "\n\n" << IOHandler::apply_neon_colors(top_border + content + bottom_border)
         << "\n" << IOHandler::reset_code;
}


// INPUT METHODS

int IOHandler::input_choose_index(int size) {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    int user_input;
    cout << "Select option: ";
    cin >> user_input;
    if (validate_index_input(user_input, size)) {
        cout << "\n";
        return user_input - 1; // convert to 0-based index
    } else {
        return input_choose_index(size);
    }
}

string IOHandler::input_choose_option(const vector<string> &options_list) {
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    int user_input;
    cout << "Select option: ";
    cin >> user_input;
    if (validate_option_input(user_input, options_list)) {
        cout << "\n";
        return options_list[user_input - 1];
    } else {
        return input_choose_option(options_list);
    }
}

void IOHandler::input_continue() {
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// COLOR METHODS

string IOHandler::apply_neon_colors(const string &text) {
    string result;
    for (char c : text) {
        if (neon_color_map.find(c) != neon_color_map.end()) {
            result += neon_color_map[c] + string(1, c) + reset_code;
        } else {
            result.push_back(c);
        }
    }
    return result;
}

void IOHandler::set_neon_color_for_char(char key, const string &ansi_code) {
    neon_color_map[key] = ansi_code;
}

string IOHandler::colorize_box(string text, string color){
    string color_code;
    try{
        color_code = IOHandler::neon_colors.at(color); 
    }
    catch(const out_of_range &e) {
        color_code = "\033[38;2;255;165;0m";
    }

    return color_code + text + IOHandler::reset_code;
}

// VALIDATION METHODS

bool IOHandler::validate_option_input(int user_input, const vector<string> &options_list) {
    if ((1 <= user_input && user_input <= (int)options_list.size()) && (!cin.fail())) {
        return true;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input, enter a number between 1 and " << options_list.size() << "\n\n";
    return false;
}

bool IOHandler::validate_index_input(int user_input, int size) {
    if ((1 <= user_input && user_input <= size) && !cin.fail()) {
        return true;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Invalid input, enter a number between 1 and " << size << "\n\n";
    return false;
}



vector<string> IOHandler::wrap_text(const string &text, size_t max_width) {
    vector<string> lines;
    vector<string> words;
    
    // Manually split text into words.
    size_t i = 0;
    while (i < text.length()) {
        // Skip any whitespace characters.
        while (i < text.length() && isspace(text[i])) {
            ++i;
        }
        if (i >= text.length())
            break;
        // Mark the start of the word.
        size_t start = i;
        // Move to the end of the word.
        while (i < text.length() && !isspace(text[i])) {
            ++i;
        }
        words.push_back(text.substr(start, i - start));
    }
    
    // Build lines by adding words until the max_width is exceeded.
    string line;
    for (const string &word : words) {
        // If the current line is empty.
        if (line.empty()) {
            // If the word itself is longer than max_width, split the word.
            if (word.size() > max_width) {
                size_t pos = 0;
                while (pos < word.size()) {
                    lines.push_back(word.substr(pos, max_width));
                    pos += max_width;
                }
            } else {
                line = word;
            }
        } else {
            // Check if adding the word (with a space) would exceed max_width.
            if (line.size() + 1 + word.size() > max_width) {
                lines.push_back(line);
                // If the new word is too long for a single line, break it up.
                if (word.size() > max_width) {
                    size_t pos = 0;
                    while (pos < word.size()) {
                        lines.push_back(word.substr(pos, max_width));
                        pos += max_width;
                    }
                    line = "";
                } else {
                    line = word;
                }
            } else {
                line += " " + word;
            }
        }
    }
    
    // Add any remaining text as the last line.
    if (!line.empty()) {
        lines.push_back(line);
    }
    
    return lines;
}