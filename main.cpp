#include <string>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <vector>
#include "trie_node.h"
#include "word_path.h"
#include "board.h"

using std::cout;
using std::cin;
using std::string;
using std::map;
using std::vector;

void build_dictionary(trie_node*);
bool check_word(string);
void print_board(word_path*);
int board_size;

int main(int argc, char** argv) {
  cout << "Building dictionary...\n";
  // build dictionary into trie
  trie_node* root = new trie_node();
  build_dictionary(root);

  cout << "Done!\n\n";

  // begin UI for solving
  cout << "Enter N for the board size (N x N):\n";
  cin >> board_size;
  word_path::b_size = board_size;

  string serialized_board;
  cout << "Enter the serialized board:\n";
  cin >> serialized_board;

  string mult_code;
  cout << "Enter multipliers divided by 2 serialized (e.g. x14 ==> 7):\n";
  cin >> mult_code;

  board* game_board = new board(serialized_board, root, mult_code, board_size);

  // print linked list of words, inserted in order of points
  string current_word;
  int count = 1;
  word_path* cursor = game_board->get_words();
  string input = "";
  vector<word_path*> buffer;
  while (input != "n" && cursor != NULL) {
    cursor = cursor->get_next_word();
    buffer.push_back(cursor);
    if (buffer.size() == 5) {
      for (int i = 0; i < buffer.size(); i++) {
        cout << "\n" << count << ". ";
        print_board(buffer[i]);
        count++;
      }
      cout << "See next 5 words? (y/n): ";
      cin >> input;
      buffer.clear();
    }
  }
  for (int i = 0; i < buffer.size(); i++) {
    cout << "\n" << count << ". ";
    print_board(cursor);
    count++;
  }
  return 0;
}


void build_dictionary(trie_node* root) {
  std::ifstream ifs ("dictionary.txt");
  if (!ifs.good()) {
    cout << "The file dictionary.txt was not found.\n";
  }
  string current;
  trie_node* cursor;
  while ((ifs >> current).good()) {
    if (!check_word(current)) {
      continue;
    }
    cursor = root;
    int count = 0;
    while (count < current.size()) {
      cursor->set_element(current[count]);
      cursor = cursor->access_by_char(current[count]);
      count++;
    }
    cursor->set_element('`');
  }
  ifs.close();
}

bool check_word(string word) {
  int char_index;
  for (int i = 0; i < word.size(); i++) {
    char_index = ((int)word[i]) - 96;
    if (char_index < 0 || char_index > 26) {
      return false;
    }
  }
  return true;
}

void print_board(word_path* wp) {
  //string border = "+-------+-------+-------+-------+";
  //string middle = "|       |       |       |       |";
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
  vector<int> indexes = wp->get_indexes();
  string word = wp->get_word();
  cout << word << ":\t" << wp->get_total() << " total points\n";
  cout << "\tMultiplier: " << wp->get_multiplier() << "\t";
  cout << "Points: " << wp->get_points() << "\t";
  cout << "Total: " << wp->get_total() << "\n";
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

