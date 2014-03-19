#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "trie_node.h"
#include "word_path.h"

using std::vector;
using std::map;
using std::cout;

class board {
  public:
    board(std::string serialized_board, trie_node* t, std::string flag_code);
    void find_paths(word_path& wp, trie_node* t, int row, int col);
    word_path* get_words() { return words; }
    word_path* find_word(int index);
    int point_flag(int row, int col) { return flags[row][col]; }
  private:
    vector< vector<char> > contents;
    vector< vector<int> > flags;
    map<char, int> point_vals;
    trie_node* root;
    word_path* words;
    void add_to_word_list(word_path& wp);
    void create_map(map<char,int>& m);
};

#endif
