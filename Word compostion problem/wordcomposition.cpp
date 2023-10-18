#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>

class TrieNode {
public:
    std::map<char, TrieNode*> children;
    bool is_end_of_word;

    TrieNode() : is_end_of_word(false) {}
};

class Trie {
public:
    TrieNode root;

    void insert(const std::string& word) {
        TrieNode* node = &root;
        for (char c : word) {
            if (node->children.find(c) == node->children.end()) {
                node->children[c] = new TrieNode();
            }
            node = node->children[c];
        }
        node->is_end_of_word = true;
    }

    std::vector<std::string> find(const std::string& prefix) {
        TrieNode* node = &root;
        for (char c : prefix) {
            if (node->children.find(c) == node->children.end()) {
                return std::vector<std::string>();
            }
            node = node->children[c];
        }
        return _find_words_with_prefix(node, prefix);
    }

private:
    std::vector<std::string> _find_words_with_prefix(TrieNode* node, const std::string& current_prefix) {
        std::vector<std::string> words;
        if (node->is_end_of_word) {
            words.push_back(current_prefix);
        }
        for (const auto& kv : node->children) {
            char c = kv.first;
            TrieNode* child = kv.second;
            std::vector<std::string> child_words = _find_words_with_prefix(child, current_prefix + c);
            words.insert(words.end(), child_words.begin(), child_words.end());
        }
        return words;
    }
};

bool prefix_checker(const std::string& word, Trie& trie) {
    int index = 1;
    while (index <= word.length()) {
        int length = trie.find(word.substr(0, index)).size();
        if (!(length > 1) && index == 1) {
            return false;
        } else if (!(length > 1)) {
            return prefix_checker(word.substr(index - 1), trie);
        } else {
            index++;
        }
    }
    return true;
}

std::string find_longest_string(std::vector<std::string>& arr) {
    return *std::max_element(arr.begin(), arr.end(), [](const std::string& a, const std::string& b) {
        return a.length() < b.length();
    });
}

std::vector<std::string> search_concat_words(const std::string& file_name, int no_of_results) {
    std::vector<std::string> result;
    Trie trie;
    std::ifstream file(file_name);

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            trie.insert(line);
        }
        file.close();

        std::vector<std::string> text;
        std::ifstream file2(file_name);
        if (file2.is_open()) {
            std::string line;
            while (std::getline(file2, line)) {
                text.push_back(line);
            }
            file2.close();
        }

        while (no_of_results > 0) {
            std::string longest_string = find_longest_string(text);
            text.erase(std::remove(text.begin(), text.end(), longest_string), text.end());

            if (prefix_checker(longest_string, trie)) {
                result.push_back(longest_string);
                no_of_results--;
            }
        }
    } else {
        std::cout << "File not found: " << file_name << std::endl;
    }

    return result;
}

int main() {
    std::vector<std::string> result1 = search_concat_words("words.txt", 2);
    if (result1.size() >= 2) {
        std::cout << result1[0] << " " << result1[1] << std::endl;
    }

    std::vector<std::string> result2 = search_concat_words("words1.txt", 2);
    if (result2.size() >= 2) {
        std::cout << result2[0] << " " << result2[1] << std::endl;
    }

    return 0;
}
