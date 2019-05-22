#ifndef ENGINE_H
#define ENGINE_H

#include <bits/stdc++.h>
#include "cache.h"
#include "constants.h"
#include "misc.h"
#include "position.h"

struct Engine {
    LRUCache<tuple<Position, int, bool>, pair<int, int>> tp_score;
    LRUCache<Position, Move> tp_move;
    int nodes, depth;

    Engine() {
        nodes = 0;
        tp_move = LRUCache<Position, Move>(TABLE_SIZE);
        tp_score = LRUCache<tuple<Position, int, bool>, pair<int, int>>(TABLE_SIZE);
    }

    int bound(Position pos, int gamma, int depth, bool root = true) {
        nodes++;
        depth = max(0, depth);
        if (pos.score <= -MATE_LOWER) return -MATE_UPPER;
        pair<int, int>* entryIt = tp_score.get(make_tuple(pos, depth, root));
        pair<int, int> entry = entryIt ? *entryIt : make_pair(-MATE_UPPER, MATE_UPPER);
        if (entry.first >= gamma && (!root || tp_move.get(pos))) {
            return entry.first;
        }
        if (entry.second < gamma) {
            return entry.second;
        }

        auto moves = [&]() {
            int res = -MATE_UPPER;
            //null move for QSearch
            if (depth > 0 && !root && pos.has_null_move()) {
                res = max(res, -bound(pos.nullmove(), 1 - gamma, depth - 3, false));
                if (res >= gamma) {
                    return res;
                }
            }

            if (depth == 0) {
                res = max(res, pos.score);
                if (res >= gamma) return res;
            }

            //killer move
            Move* killerIt = tp_move.get(pos);
            if (killerIt && (depth > 0 || pos.value(*killerIt) >= QS_LIMIT)) {
                Move killer = *killerIt;
                res = max(res, -bound(pos.move(killer), 1 - gamma, depth - 1, false));
                if (res >= gamma) {
                    //cerr << "tp_move set " << pos.board << ' ' << killer.from << ' ' << killer.to << endl;
                    tp_move.set(pos, killer);
                    return res;
                }
            }

            //all moves
            vector<Move> moves = pos.gen_moves();
            sort(moves.begin(), moves.end(), [&pos](const Move &u, const Move &v) {
                return pos.value(u) > pos.value(v);
            });
            for (Move move : moves) {
                if (depth > 0 || pos.value(move) >= QS_LIMIT) {
                    res = max(res, -bound(pos.move(move), 1 - gamma, depth - 1, false));
                    if (res >= gamma) {
                        //cerr << "tp_move set " << pos.board << ' ' << depth << ' ' << move.from << ' ' << move.to << endl;
                        tp_move.set(pos, move);
                        return res;
                    }
                }
            }
            return res;
        };

        int best = moves();

        // check draw
        if (best < gamma && best < 0 && depth > 0) {
            auto is_dead = [&moves](Position pos) {
                for (auto m : pos.gen_moves()) {
                    if (pos.value(m) >= MATE_LOWER) return 1;
                }
                return 0;
            };
            auto check_all_dead = [&]() {
                for (auto m : pos.gen_moves()) {
                    if (!is_dead(pos.move(m))) return 0;
                }
                return 1;
            };
            if (check_all_dead()) {
                bool in_check = is_dead(pos.nullmove());
                best = in_check ? -MATE_UPPER : 0;
            }
        }

        if (best >= gamma) {
            tp_score.set(make_tuple(pos, depth, root), {best, entry.second});
        } else {
            tp_score.set(make_tuple(pos, depth, root), {entry.first, best});
        }
        return best;
    }

    void search(const Position &pos, int depth) {
        this->depth = depth;
        int lower = -MATE_UPPER, upper = MATE_UPPER;
        while (lower < upper - EVAL_ROUGHNESS) {
            int gamma = (lower + upper + 1) >> 1;
            int score = bound(pos, gamma, depth);
            if (score >= gamma) lower = score;
            else upper = score;
        }
        bound(pos, lower, depth);
    }
};


#endif