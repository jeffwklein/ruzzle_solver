#ifndef WORD_PATH_H
#define WORD_PATH_H

#include <string>
#include <iostream>
#include "trie_node.h"

using std::vector;
using std::cout;

class word_path {
  public:
    word_path(std::string init_word,
        vector<int> init_indexes,
        vector< vector<bool> > init_used, 
        int init_pts = 0, 
        word_path* init_next = NULL,
        int init_mult = 1);
    std::string get_word() { return word; }
    vector<int> get_indexes() { return indexes; }
    vector< vector<bool> > get_used() { return used; }
    int get_points();
    word_path* get_next_word() { return next_word; }
    // ^^calculates and returns point total
    void add_points(int char_val, int flag);
    void undo_points(int char_val, int flag);
    void set_next_word(word_path* next) { next_word = next; }
    bool move_forward(char c, int row, int col);
    // ^^^ append char, set&check bool, push back index
    void undo_move(int char_val, int flag);
    // undo everything that move_forward does
  private:
    std::string word;
    vector<int> indexes;
    vector< vector<bool> > used;
    int points;
    int multiplier;
    word_path* next_word;
};

#endif
