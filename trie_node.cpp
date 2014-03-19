#include <string>
#include <vector>
#include <iostream>
#include "trie_node.h"

using std::cout;

trie_node::trie_node() {
  for (int i = 0; i < 27; i++) {
    elements.push_back(NULL);
  }
}

bool trie_node::search(std::string key) {
  if (key.size() < 1) {
    return false;
  }
  trie_node* cursor;
  cursor = access_by_char(key[0]);
  int count = 1;
  while (count < key.size()) {
    if (cursor == NULL) {
      return false;
    }
    cursor = cursor->access_by_char(key[count]);
    count++;
  }
  if (cursor == NULL) {
    return false;
  }
  if (cursor->access_by_index(0) != NULL) {
    return true;
  }
  else {
    return false;
  }
}

trie_node* trie_node::access_by_index(int index) {
  return (index < 0 || index > 26) ? NULL : elements[index];
}

trie_node* trie_node::access_by_char(char value) {
  return access_by_index( ((int)value) - 96 );
}

void trie_node::set_element(char value) {
  int index = ((int)value) - 96;
  if (elements[index] == NULL) {
    elements[index] = new trie_node();
  }
}
