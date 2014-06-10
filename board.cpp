#include "board.h"
#include "trie_node.h"
#include "word_path.h"

board::board(std::string serialized_board, trie_node* t,
    std::string flag_code, int size)
    : board_size(size), root(t), words(NULL) {
  for (int i = 0; i < board_size; i++) {
    vector<char> row;
    vector<int> flag_row;
    for (int j = 0; j < board_size; j++) {
      row.push_back( serialized_board[(i*board_size)+j] );
      int mult = ((int)flag_code[(i*board_size)+j] - 48) * 2;
      flag_row.push_back(mult);
    }
    contents.push_back(row);
    flags.push_back(flag_row);
  }
  create_map(point_vals);
  for (int i = 0; i < board_size; i++) {
    for (int j = 0; j < board_size; j++) {
      std::string s;
      s = "";
      std::vector<int> v_int;
      std::vector< std::vector<bool> > v_bool;
      word_path wp(s, v_int, v_bool);
      wp.move_forward(contents[i][j], i, j);
      wp.add_points(point_vals[contents[i][j]], point_flag(i, j));
      find_paths(wp, root->access_by_char(contents[i][j]), i, j);
      wp.undo_move(point_vals[contents[i][j]], point_flag(i, j));
    }
  }
}

void board::find_paths(word_path& wp, trie_node* cursor, int r, int c) {
  if (cursor == NULL) {
    return;
  }
  if (cursor->access_by_index(0) != NULL) {
    add_to_word_list(wp);
  }
  //north
  if (r > 0) {
    if (wp.move_forward(contents[r-1][c], r-1, c)) {
      wp.add_points(point_vals[contents[r-1][c]], point_flag(r-1, c));
      find_paths(wp, cursor->access_by_char(contents[r-1][c]), r-1, c);
      wp.undo_move(point_vals[contents[r-1][c]], point_flag(r-1, c));
    }
  }
  //northeast
  if (r > 0 && c < board_size-1) {
    if (wp.move_forward(contents[r-1][c+1], r-1, c+1)) {
      wp.add_points(point_vals[contents[r-1][c+1]], point_flag(r-1, c+1));
      find_paths(wp, cursor->access_by_char(contents[r-1][c+1]), r-1, c+1);
      wp.undo_move(point_vals[contents[r-1][c+1]], point_flag(r-1, c+1));
    }
  }
  //east
  if (c < board_size-1) {
    if (wp.move_forward(contents[r][c+1], r, c+1)) {
      wp.add_points(point_vals[contents[r][c+1]], point_flag(r, c+1));
      find_paths(wp, cursor->access_by_char(contents[r][c+1]), r, c+1);
      wp.undo_move(point_vals[contents[r][c+1]], point_flag(r, c+1));
    }
  }
  //southeast
  if (r < board_size-1 && c < board_size-1) {
    if (wp.move_forward(contents[r+1][c+1], r+1, c+1)) {
      wp.add_points(point_vals[contents[r+1][c+1]], point_flag(r+1, c+1));
      find_paths(wp, cursor->access_by_char(contents[r+1][c+1]), r+1, c+1);
      wp.undo_move(point_vals[contents[r+1][c+1]], point_flag(r+1, c+1));
    }
  }
  //south
  if (r < board_size-1) {
    if (wp.move_forward(contents[r+1][c], r+1, c)) {
      wp.add_points(point_vals[contents[r+1][c]], point_flag(r+1, c));
      find_paths(wp, cursor->access_by_char(contents[r+1][c]), r+1, c);
      wp.undo_move(point_vals[contents[r+1][c]], point_flag(r+1, c));
    }
  }
  //southwest
  if (r < board_size-1 && c > 0) {
    if (wp.move_forward(contents[r+1][c-1], r+1, c-1)) {
      wp.add_points(point_vals[contents[r+1][c-1]], point_flag(r+1, c-1));
      find_paths(wp, cursor->access_by_char(contents[r+1][c-1]), r+1, c-1);
      wp.undo_move(point_vals[contents[r+1][c-1]], point_flag(r+1, c-1));
    }
  }
  //west
  if (c > 0) {
    if (wp.move_forward(contents[r][c-1], r, c-1)) {
      wp.add_points(point_vals[contents[r][c-1]], point_flag(r, c-1));
      find_paths(wp, cursor->access_by_char(contents[r][c-1]), r, c-1);
      wp.undo_move(point_vals[contents[r][c-1]], point_flag(r, c-1));
    }
  }
  //northwest
  if (r > 0 && c > 0) {
    if (wp.move_forward(contents[r-1][c-1], r-1, c-1)) {
      wp.add_points(point_vals[contents[r-1][c-1]], point_flag(r-1, c-1));
      find_paths(wp, cursor->access_by_char(contents[r-1][c-1]), r-1, c-1);
      wp.undo_move(point_vals[contents[r-1][c-1]], point_flag(r-1, c-1));
    }
  }
}

void board::add_to_word_list(word_path& wp) {
  // first item in list
  if (words == NULL) {
    words = new word_path(wp.get_word(),
        wp.get_indexes(), wp.get_used(),
        wp.get_points());
    return;
  }
  // new head of list
  if (wp.get_total() >= words->get_total()) {
    words = new word_path(wp.get_word(),
        wp.get_indexes(), wp.get_used(),
        wp.get_points(), wp.get_multiplier(), words);
    return;
  }
  // otherwise
  word_path* cursor;
  for (cursor = words; cursor != NULL; cursor = cursor->get_next_word() ) {
    // new end of list
    if (cursor->get_next_word() == NULL) {
      word_path* new_word = new word_path(wp.get_word(),
          wp.get_indexes(), wp.get_used(), wp.get_points(), wp.get_multiplier());
      cursor->set_next_word(new_word);
      return;
    }
    // insert at proper position in list
    if (wp.get_total() > cursor->get_next_word()->get_total()) {
      word_path* new_word = new word_path(wp.get_word(),
          wp.get_indexes(), wp.get_used(), wp.get_points(),
          wp.get_multiplier(), cursor->get_next_word());
      cursor->set_next_word(new_word);
      return;
    }
  }
}

word_path* board::find_word(int index) {
  word_path* cursor = words;
  int count = 1;
  while (cursor != NULL) {
    if (count == index) {
      return cursor;
    }
    cursor = cursor->get_next_word();
    count++;
  }
  return cursor;
}

void board::create_map(map<char,int>& m) {
  m['a'] = 1;
  m['b'] = 4;
  m['c'] = 4;
  m['d'] = 2;
  m['e'] = 1;
  m['f'] = 4;
  m['g'] = 3;
  m['h'] = 4;
  m['i'] = 1;
  m['j'] = 10;
  m['k'] = 5;
  m['l'] = 1;
  m['m'] = 3;
  m['n'] = 1;
  m['o'] = 1;
  m['p'] = 4;
  m['q'] = -1000;
  m['r'] = 1;
  m['s'] = 1;
  m['t'] = 1;
  m['u'] = 2;
  m['v'] = 4;
  m['w'] = 4;
  m['x'] = 8;
  m['y'] = 4;
  m['z'] = 10;
}
