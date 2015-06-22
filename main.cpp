#include <string>
#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <map>
#include <vector>
#include "trie.h"
#include "word_path.h"
#include "board.h"

using std::cout;
using std::cin;
using std::string;
using std::map;
using std::vector;

void print_board(word_path*);

int main(int argc, char** argv) {
  // build dictionary into trie
  trie* dict = new trie();
  dict->build_dictionary("dictionary.txt");

  // begin UI for solving
  cout << "Enter N for the board size (N x N):\n";
  int board_size;
  cin >> board_size;
  word_path::board_size = board_size;
  int serial_length = board_size * board_size;

  string serialized_board;
  do {
    cout << "\nEnter the serialized board:\n";
    cin >> serialized_board;
  } while ((serialized_board.length() != serial_length) &&
      (cout << "Incorrect length given N. Should be " << serial_length << " characters.\n") );

  string mult_code;
  cout << "\nEnter multipliers divided by 2 serialized (or \"n\" to skip):\n";
  cin >> mult_code;
  if (mult_code == "n") mult_code = string(serial_length, '0');

  int words_needed;
  cout << "Enter words needed (0 if doesn't apply):";
  cin >> words_needed;

  board* game_board = new board(serialized_board, dict, mult_code, board_size);

  // print linked list of words, inserted in order of points
  string current_word;
  int count = 1, count_sets = 0, count_points = 0;
  word_path* cursor = game_board->get_words();
  string input = "";
  int top_15_points = 0, top_15_count = 0;
  vector<word_path*> all_words, print_words;
  while (input != "n" && cursor != NULL) {
    cursor = cursor->get_next_word();
    all_words.push_back(cursor);
    if (all_words.size() % 30 == 0) {
      int repeat_count = 0;
      for (int i = (count_sets*30); i < all_words.size(); i++) {
        bool do_print = true;
        // compare to previously printed items to check for duplicates
        for (int j = 0; j < i; j++) {
          if (all_words[i]->get_word() == all_words[j]->get_word()) {
            do_print = false;
            repeat_count++;
          } else if (top_15_count < 15) {
            top_15_points += all_words[j]->get_total();
            top_15_count++;
          }
        }
        if (do_print){
          cout << "\n" << count << ". ";
          all_words[i]->print_board();
          count++;
        } else {
          cout << "REPEATED WORD: " << all_words[i]->get_word() << "\n\n";
        }
      }
      cout << "Total repeats: " << repeat_count << "\n";
      cout << "Top 15 total score: " << top_15_points << "\n";
      repeat_count = 0;
      cout << "See next word? (y/n): ";
      cin >> input;
      count_sets++;
    }
  }
  // print remaining items if not a multiple of 5
  /* 
  for (int i = 0; i < all_words.size(); i++) {
    cout << "\n" << count << ". ";
    print_board(cursor);
    count++;
  }
  */
  return 0;
}

/* Prints an ASCII representation of the word's path
 * on the board.
 */

