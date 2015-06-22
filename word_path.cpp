#include <vector>
#include <string>
#include <iostream>
#include "word_path.h"

using std::string;

word_path::word_path( string init_word,
    vector<int> init_indexes, 
    vector< vector<bool> > init_used,
    int init_points,
    int init_mult,
    word_path* init_next ) : word(init_word),
      indexes(init_indexes),
      points(init_points),
      next_word(init_next), multiplier(init_mult) {
  if (init_used.size() == 0) {
    for (int i = 0; i < board_size; i++) {
      vector<bool> row;
      for (int j = 0; j < board_size; j++) {
        row.push_back(false);
      }
      used.push_back(row);
    }
  }
}

int word_path::board_size = 0;

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

void word_path::print_board() {
  string border = "";
  string middle = "";
  for (int i = 0; i < board_size; i++) {
    border += "+-------";
    middle += "|       ";
  }
  border += "+";
  middle += "|";
  vector<string> picture;
  for (int i = 0; i < board_size; i++) {
    picture.push_back(border);
    for (int j = 0; j < 3; j++) {
      picture.push_back(middle);
    }
  }
  picture.push_back(border);
  int row_change = 0, col_change = 0;
  int row, col, next_row, next_col;
  cout << word << ":\t" << get_total() << " total points\n";
  cout << "\tMultiplier: " << multiplier << "\t";
  cout << "Points: " << points << "\t";
  cout << "Total: " << get_total() << "\n";
  for (int i = 0; i < indexes.size(); i++) {
    col = indexes[i]%10;
    row = (indexes[i]-col)/10;
    picture[(row*4)+2][(col*8)+4] = word[i];
    if (i == 0) {
      picture[(row*4)+1][(col*8)+2] = '*';
      picture[(row*4)+1][(col*8)+4] = '*';
      picture[(row*4)+1][(col*8)+6] = '*';
      picture[(row*4)+2][(col*8)+2] = '*';
      // capitalize start letter on path illustration
      //picture[(row*4)+2][(col*8)+4] = (char)((int)word[i] - 32);
      picture[(row*4)+2][(col*8)+6] = '*';
      picture[(row*4)+3][(col*8)+2] = '*';
      picture[(row*4)+3][(col*8)+4] = '*';
      picture[(row*4)+3][(col*8)+6] = '*';
    }
    if (i == indexes.size()-1) {
      break;
    }
    next_col = indexes[i+1]%10;
    next_row = (indexes[i+1]-next_col)/10;
    col_change = next_col - col;
    row_change = next_row - row;
    //north
    if (row_change == -1 && col_change == 0) {
      picture[(row*4)+1][(col*8)+4] = '|';
      picture[(row*4)-1][(col*8)+4] = '|';
    }
    //northeast
    else if (row_change == -1 && col_change == 1) {
      picture[(row*4)+1][(col*8)+6] = '/';
      picture[(row*4)-1][(col*8)+10] = '/';
    }
    //east
    else if (row_change == 0 && col_change == 1) {
      picture[(row*4)+2][(col*8)+6] = '-';
      picture[(row*4)+2][(col*8)+10] = '-';
    }
    //southeast
    else if (row_change == 1 && col_change == 1) {
      picture[(row*4)+3][(col*8)+6] = '\\';
      picture[(row*4)+5][(col*8)+10] = '\\';
    }
    //south
    else if (row_change == 1 && col_change == 0) {
      picture[(row*4)+3][(col*8)+4] = '|';
      picture[(row*4)+5][(col*8)+4] = '|';
    }
    //southwest
    else if (row_change == 1 && col_change == -1) {
      picture[(row*4)+3][(col*8)+2] = '/';
      picture[(row*4)+5][(col*8)-2] = '/';
    }
    //west
    else if (row_change == 0 && col_change == -1) {
      picture[(row*4)+2][(col*8)+2] = '-';
      picture[(row*4)+2][(col*8)-2] = '-';
    }
    //northwest
    else if (row_change == -1 && col_change == -1) {
      picture[(row*4)+1][(col*8)+2] = '\\';
      picture[(row*4)-1][(col*8)-2] = '\\';
    }
  }
  for (int i = 0; i < picture.size(); i++) {
    cout << picture[i] << "\n";
  }
}

