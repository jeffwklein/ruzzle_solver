#ifndef TRIE_H
#define TRIE_H
#include <string>
#include "trie_node.h"

class trie {
  public:
    trie();
    trie(std::string filename);
    ~trie();
    bool build_dictionary(std::string filename);
    bool search(std::string key);
    trie_node* get_root() { return root; }
  private:
    trie_node* root;
    // helper method for build_dictionary
    bool check_word(std::string word);
};

#endif
