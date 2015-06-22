#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "trie.h"
#include "word_path.h"

using std::vector;
using std::map;
using std::cout;

// dirty hack to cover cross-dependency issue

class board {
  public:
    board(std::string serialized_board, trie* t, std::string flag_code, int size = 4);
    void find_paths(word_path& wp, trie_node* t, int row, int col);
    word_path* get_words() { return words; }
    word_path* find_word(int index);
    int point_flag(int row, int col) { return flags[row][col]; }
  private:
    int board_size;
    vector< vector<char> > contents;
    vector< vector<int> > flags;
    map<char, int> point_vals;
    trie* dict;
    word_path* words;
    void add_to_word_list(word_path& wp);
    void create_map(map<char,int>& m);
};

#endif
