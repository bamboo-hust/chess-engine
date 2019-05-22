#include <bits/stdc++.h>
//#include "position.h"
#include "misc.h"
#include "constants.h"

using namespace std;

const string FEN_INITIAL = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const bool WHITE = 0;
const bool BLACK = 1;

const int N = 1e6 + 5;
map < string, int > posWords;

string stacks[N];
string keywords[] = {"quit", "new", "setboard", "force", "go", "ping", "usermove", "time", 
                     "otim", "perft", "post", "nopost", "xboard", "random", "hard", "accept",
                     "level"};
const int KEYWORDS_SIZE = 17;


void init() {
    for (int i = 0; i < KEYWORDS_SIZE; i++) {
        posWords[keywords[i]] = i + 1;
    }
}

string detect(string command) {
    string prefix = "";
    for (int i = 0; i < command.size(); i++) {
        prefix = prefix + command[i];
        if (posWords[prefix] != 0) {
            return prefix;
        }
    }
    return "";
}


string read_next_command() {
    string command;
    getline(cin, command);
    return command;
}

int parse(string ) {

}


Move mparse(string move) {
    
}

int main() {
    init();
    string pos = FEN_INITIAL;
    bool forced = false;
    bool color = WHITE;
    int our_time = 1000, opp_time = 1000;
    bool show_thinking = false;
    int top = 0;
    while (true) {
        string command;
        if (top > 0) {
            command = stacks[top];
            top--;
        } else {
            command = read_next_command();
        }
        if (command == "quit") {
            break;
        }
        string prefix = detect(command);
        if (prefix == "") {
            cout << "Error (unkown command):" << command << endl;
            continue;
        }
        string params = command.substr(prefix.size() + 1, command.size() - prefix.size() - 1);
        if (prefix == "new") {
            stacks[++top] = "setboard" + FEN_INITIAL;
        } else if (prefix == "setboard") {
            // pos = smove
        } else if (prefix == "force") {

        } else if (prefix == "go") {

        } else if (prefix == "ping") {

        } else if (prefix == "usermove") {
            cout << params << endl;
            break;
        } else if (prefix == "time") {
            our_time = atoi(params.c_str());
            cout << our_time << endl;
        } else if (prefix == "otime") {
            opp_time = atoi(params.c_str());
        } else {
            continue;
        }
    }
}