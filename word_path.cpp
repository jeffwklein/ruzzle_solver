#include <vector>
#include <string>
#include <iostream>
#include "word_path.h"

word_path::word_path( std::string init_word,
    vector<int> init_indexes, 
    vector< vector<bool> > init_used,
    int init_points,
    int init_mult,
    word_path* init_next ) : word(init_word),
      indexes(init_indexes),
      points(init_points),
      next_word(init_next), multiplier(init_mult) {
  if (init_used.size() == 0) {
    for (int i = 0; i < b_size; i++) {
      vector<bool> row;
      for (int j = 0; j < b_size; j++) {
        row.push_back(false);
      }
      used.push_back(row);
    }
  }
}

int word_path::b_size = 0;

int word_path::get_total() {
  if (multiplier == 0) return points;
  return points * multiplier;
}

bool word_path::move_forward(char c, int row, int col) {
  if (used[row][col] == true) {
    return false;
  }
  used[row][col] = true;
  word.append(1, c);
  indexes.push_back((row*10)+col);
  return true;
}

void word_path::undo_move(int char_val, int flag) {
  int i = indexes.back();
  indexes.pop_back();
  int col = i%10;
  int row = (i-col)/10;
  used[row][col] = false;
  word.erase(word.size()-1, 1);
  points -= char_val;
  multiplier -= flag;
}

void word_path::add_points(int char_val, int flag) {
  points += char_val;
  multiplier += flag;
}



