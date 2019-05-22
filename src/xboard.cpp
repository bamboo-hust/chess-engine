#include <bits/stdc++.h>

using namespace std;

#include "position.h"
#include "misc.h"
#include "constants.h"

const string FEN_INITIAL = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const bool WHITE = 0;
const bool BLACK = 1;

const int MAX_STACK = 1e6 + 5;
map < string, int > posWords;

string stacks[MAX_STACK];
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

int parse(string move) {
    int fil = move[0] - 'a';
    int rank = move[1] - '1';
    return A1 + fil - 10 * rank;
}


bool forced = false;
bool color = WHITE;
int our_time = 1000, opp_time = 1000;
bool show_thinking = false;
int top = 0;

Move mparse(string move) {
    Move m = Move(parse(move.substr(0, 2)), parse(move.substr(2, 2)));
    if (color == WHITE) {
        return m;
    }
    return Move(119 - m.from, 119 - m.to);
}


string parse_board(string fen) {
    int numSpaces = 21;
    string board = "";
    for (int i = 0; i < numSpaces; i++) {
        board = board + " ";
    }
    for (int i = 0; i < fen.size(); i++) {
        if (fen[i] == '/') {
            board = board + "  ";
            continue;
        }
        if (fen[i] >= '0' && fen[i] <= '9') {
            int numDots = fen[i] - '0';
            while (numDots--) {
                board = board + '.';
            }
            continue;
        }
        board = board + fen[i];
    }
    for (int i = 0; i < numSpaces; i++) {
        board = board + " ";
    }
    for (int i = 9; i < board.size(); i += 10) {
        board[i] = '\n';
    }
    return board;
}

vector< string > split(string fen) {
    vector< string > fens;
    string cur = "";
    for (int i = 0; i < fen.size(); i++) {
        if (fen[i] == ' ') {
            fens.push_back(cur);
            cur = "";
        } else {
            cur = cur + fen[i];
        }
    }
    if (cur.size() > 0) {
        fens.push_back(cur);
    }
    return fens;
}

Position parseFEN(string fen) {
    vector< string > fens = split(fen);
    string board = parse_board(fens[0]);
    color = fens[1] == "w" ? WHITE : BLACK;
    string castling = fens[2];
    array<bool, 2> wc = {false, false};
    array<bool, 2> bc = {false, false};
    for (char i : castling) {
        wc[0] = wc[0] | (i == 'Q');
        wc[1] = wc[1] | (i == 'K');
        bc[0] = bc[0] | (i == 'k');
        bc[1] = bc[1] | (i == 'q');
    }
    int ep = (fens[3] != "-");

    Position pos = Position{board, color, wc, bc, ep, 0};
    return pos;
    
}

int main() {
    init();
    Position pos = parseFEN(FEN_INITIAL);
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
        vector < string > splits = split(command);
        string params = splits.size() >= 2 ? splits[1] : "";
        if (prefix == "new") {
            stacks[++top] = "setboard" + FEN_INITIAL;
        } else if (prefix == "setboard") {
            pos = parseFEN(params);
        } else if (prefix == "force") {
            forced = true;
        } else if (prefix == "go") {
            
        } else if (prefix == "ping") {
            cout << "pong" << " " << params;
        } else if (prefix == "usermove") {
            cout << pos.board << endl;
            Move m = mparse(params);
            pos = pos.move(m);
            cout << pos.board << endl;
            color = 1 - color;
            if (!forced) {
                stacks[++top] = "go";
            }
        } else if (prefix == "time") {
            our_time = atoi(params.c_str());
            cout << our_time << endl;
        } else if (prefix == "otime") {
            opp_time = atoi(params.c_str());
        } else if (prefix == "post") {
            show_thinking = true;
        } else if (prefix == "nopost") {
            show_thinking = false;
        } else {
            continue;
        }
    }
}