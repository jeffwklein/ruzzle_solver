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

int main(int argc, char** argv) {
  cout << "Building dictionary...\n";
  // build dictionary into trie
  trie_node* root = new trie_node();
  build_dictionary(root);

  cout << "Done!\n\n";

  // begin UI for solving
  string serialized_board;
  cout << "Enter the serialized board:\n";
  cin >> serialized_board;

  string flag_code;
  cout << "Enter serialized flag codes.\n";
  cout << "\t0: Regular character.\n";
  cout << "\t1: Double letter score.\n";
  cout << "\t2: Triple letter score.\n";
  cout << "\t3: Double word score.\n";
  cout << "\t4: Triple word score.\n";
  cin >> flag_code;

  board* game_board = new board(serialized_board, root, flag_code);

  // print linked list of words, inserted in order of points
  string current_word;
  int count = 0;
  word_path* cursor = game_board->get_words();
  string input = "";
  while (input != "n" && cursor != NULL) {
    cout << "\n" << ++count << ". ";
    print_board(cursor);
    cursor = cursor->get_next_word();
    cout << "See next word? (y/n): ";
    cin >> input;
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
  string border = "+-------+-------+-------+-------+";
  string middle = "|       |       |       |       |";
  vector<string> picture;
  for (int i = 0; i < 4; i++) {
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
  cout << word << ":\t" << wp->get_points() << " points\n";
  for (int i = 0; i < indexes.size(); i++) {
    col = indexes[i]%10;
    row = (indexes[i]-col)/10;
    picture[(row*4)+2][(col*8)+4] = word[i];
    if (i == 0) {
      picture[(row*4)+1][(col*8)+2] = '*';
      picture[(row*4)+1][(col*8)+4] = '*';
      picture[(row*4)+1][(col*8)+6] = '*';
      picture[(row*4)+2][(col*8)+2] = '*';
      picture[(row*4)+2][(col*8)+4] = (char)(((int)word[i])-32);
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

