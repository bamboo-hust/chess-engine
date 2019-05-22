#include <bits/stdc++.h>

using namespace std;

#include "position.h"
#include "misc.h"
#include "constants.h"
#include "engine.h"

const string FEN_INITIAL = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
const bool WHITE = 0;
const bool BLACK = 1;

const int MAX_STACK = 1e6 + 5;
map < string, int > posWords;

string stacks[MAX_STACK];
string keywords[] = {"quit", "new", "setboard", "force", "go", "ping", "usermove", "time", 
                     "otim", "perft", "post", "nopost", "xboard", "random", "hard", "accept",
                     "level", "protover 2"};
const int KEYWORDS_SIZE = 18;


void init() {
    init_constants();
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
    cout.flush();
    return command;
}

int parse(string move) {
    int fil = move[0] - 'a';
    int rank = move[1] - '1';
    return A1 + fil - 10 * rank;
}

string render(int i) {
    int rank = (i - A1) / 10;
    int fil = (i - A1) % 10;
    if (fil < 0) {
        rank = rank - 1;
        fil  = fil + 10;
    }
    return string("") + (char)('a' + fil) + (char)('0' + (-rank + 1));
}


bool forced = false;
bool color = WHITE;
int our_time = 10, opp_time = 10;
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

    int score = 0;
    for (int i = 0; i < board.size(); ++i)
        if (Utils::is_uppercase(board[i])) score += pst[board[i]][i];
    for (int i = 0; i < board.size(); ++i)
        if (Utils::is_lowercase(board[i])) score -= pst[Utils::swapcase(board[i])][119 - i];


    Position pos = Position(board, score, wc, bc, 0, 0);
    if (color == BLACK) return pos.rotate();
    return pos;
    
}


bool get_color(Position pos) {
    if (pos.board[0] == '\n') {
        return BLACK;
    }
    return WHITE;
}

string mrender(Position pos, Move m) {
    string p = "";
    if (A8 <= m.to && m.to <= H8 && pos.board[m.from] == 'P') {
        p = 'q';
    }
    if (get_color(pos) == BLACK) {
        m = Move(119 - m.from, 119 - m.to);
    }
    return render(m.from) + render(m.to) + p;
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
        if (command == "protover 2") {
            cout << "feature done=0" << endl;
            cout << "feature myname=\"chess\"" << endl;
            cout << "feature usermove=1" << endl;
            cout << "feature setboard=1" << endl;
            cout << "feature ping=1" << endl;
            cout << "feature sigint=0" << endl;
            cout << "feature variants=\"normal\"" << endl;
            cout << "feature done=1" << endl;
        } else if (command == "new") {
            stacks[++top] = "setboard " + FEN_INITIAL;
        } else if (prefix == "setboard") {
            pos = parseFEN(command.substr(prefix.size() + 1, command.size() - prefix.size() - 1));
        } else if (command == "force") {
            forced = true;
        } else if (command == "go") {
            forced = false;
            clock_t start = clock();
            int move_remains = 40;
            double use = our_time / move_remains;
            if (our_time >= 100 && opp_time >= 100) {
                use *= our_time / opp_time;
            }
            Engine e;
            for (int depth = 1; depth <= MAX_DEPTH; depth++) {
                e.search(pos, depth);
                if (clock() - start / CLOCKS_PER_SEC > use) {
                    break;
                }
            }
            Move *pointer = e.tp_move.get(pos);
            assert(pointer != NULL);
            Move m = *pointer;
            cout << "move " << mrender(pos, m) << endl;
            pos = pos.move(m);
            color = 1 - color;
            //cout << (clock() - start) / CLOCKS_PER_SEC << endl;
            
        } else if (prefix == "ping") {
            cout << "pong" << " " << split(command)[1];
        } else if (prefix == "usermove") {
            Move m = mparse(split(command)[1]);
            pos = pos.move(m);
            //cout << pos.board << endl;
            color = 1 - color;
            if (!forced) {
                stacks[++top] = "go";
            }
        } else if (prefix == "time") {
            our_time = atoi(split(command)[1].c_str());
        } else if (prefix == "otime") {
            opp_time = atoi(split(command)[1].c_str());
        } else if (prefix == "post") {
            show_thinking = true;
        } else if (prefix == "nopost") {
            show_thinking = false;
        } else {
            continue;
        }
    }
}