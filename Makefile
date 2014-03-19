compile:
	g++ main.cpp trie_node.cpp board.cpp word_path.cpp -o solve

debug:
	g++ -g main.cpp trie_node.cpp board.cpp word_path.cpp -o solve

clean:
	rm solve


