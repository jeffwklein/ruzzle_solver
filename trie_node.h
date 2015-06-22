#ifndef TRIE_NODE_H
#define TRIE_NODE_H
#include <string>
#include <vector>

class trie_node {
  public:
    trie_node();
    ~trie_node();
    bool search(std::string key);
    trie_node* access_by_index(int index);
    trie_node* access_by_char(char value);
    void set_element(char value);
  private:
    std::vector<trie_node*> elements;
};

#endif
