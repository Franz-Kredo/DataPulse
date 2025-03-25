#ifndef IOHANDLER_H
#define IOHANDLER_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

// using std::string;
// using std::vector;
// using std::unordered_map;

using namespace std;


class IOHandler {
public:
    // Static variables accessible to many functions
    static unordered_map<char, string> neon_color_map;
    static const string reset_code;
    static const unordered_map<string, string> neon_colors;
    static int title_max_len;
    static int subtitle_max_len;
    static int msg_padding_len;

    // OUTPUT METHODS
    static void clear_terminal();
    static void wait(unsigned int seconds);
    static void output_title(const string &title, string color="orange");
    static void output_subtitle(const string &subtitle, string color="blue");
    static void output_msg(const string &msg);
    static void output_options(const string &options_title, const vector<string> &options_list);

    static void display_box_layout(const string &title, const vector<string> &output, const string &outline_color = "pink", const string &content_color = "white", int size=135);

    // static void write_dialoge(string dialog);
    // static void write_story(string dialog);
    // INPUT METHODS
    static int input_choose_index(int size);
    static string input_choose_option(const vector<string> &options_list);
    static void input_continue();

    // COLOR METHODS
    static string apply_neon_colors(const string &text);
    static void set_neon_color_for_char(char key, const string &ansi_code);
    static string colorize_box(string text, string color);


    static vector<string> wrap_text(const std::string &text, size_t maxWidth);


private:
    // VALIDATION METHODS
    static bool validate_option_input(int user_input, const vector<string> &options_list);
    static bool validate_index_input(int user_input, int size);
};

#endif // IOHANDLER_H