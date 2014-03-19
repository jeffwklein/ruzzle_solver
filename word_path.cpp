#include <vector>
#include <string>
#include <iostream>
#include "word_path.h"


word_path::word_path( std::string init_word,
    vector<int> init_indexes, 
    vector< vector<bool> > init_used,
    int init_points,
    word_path* init_next,
    int init_mult ) : word(init_word),
      indexes(init_indexes),
      points(init_points),
      next_word(init_next), multiplier(init_mult) {
  if (init_used.size() == 0) {
    for (int i = 0; i < 4; i++) {
      vector<bool> row;
      for (int j = 0; j < 4; j++) {
        row.push_back(false);
      }
      used.push_back(row);
    }
  }
}

int word_path::get_points() {
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
  if (flag == 0) {
    points -= char_val;
  }
  else if (flag == 1) {
    points -= (2*char_val);
  }
  else if (flag == 2) {
    points -= (3*char_val);
  }
  else if (flag == 3) {
    points -= char_val;
    multiplier /= 2;
  }
  else if (flag == 4) {
    points -= char_val;
    multiplier /= 3;
  }
}

void word_path::add_points(int char_val, int flag) {
  if (flag == 0) {
    points += char_val;
  }
  else if (flag == 1) {
    points += (2*char_val);
  }
  else if (flag == 2) {
    points += (3*char_val);
  }
  else if (flag == 3) {
    points += char_val;
    multiplier *= 2;
  }
  else if (flag == 4) {
    points += char_val;
    multiplier *= 3;
  }
}


