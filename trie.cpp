#include <string>
#include <iostream>
#include <fstream>
#include "trie.h"
#include "trie_node.h"

using std::cout;
using std::string;

trie::trie() : root(NULL) {}

trie::trie(string filename) {
  root = new trie_node();
  build_dictionary(filename);
}

bool trie::build_dictionary(string filename) {
  if (root == NULL) {
    root = new trie_node();
  }
  std::ifstream ifs (filename);
  if (!ifs.good()) {
    cout << "The file "<< filename <<" was not found.\n";
    return false;
  }
  cout << "Building dictionary...";
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
  cout << "Done!\n";
  return true;
}

bool trie::search(string key) {
  return root->search(key);
}

bool trie::check_word(string word) {
  int char_index;
  for (int i = 0; i < word.size(); i++) {
    char_index = ((int)word[i]) - 96;
    if (char_index < 0 || char_index > 26) {
      return false;
    }
  }
  return true;
}
