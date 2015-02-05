#ifndef _HUFFMAN_ENCODER_H
#define _HUFFMAN_ENCODER_H

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

using AlphabetSymbol = std::string;
using EncodedSymbol = std::vector<bool>;
using HuffmanTable = std::unordered_map<AlphabetSymbol, EncodedSymbol>;

#define BYTE_SIZE 8

typedef struct SFEntry {
  AlphabetSymbol symbol;
  float freq;
} SFEntry;


class HuffmanEncoder {
 private:
  class HTNode {
   private:
    AlphabetSymbol symbol;
    float freq;
    std::shared_ptr<HTNode> left;
    std::shared_ptr<HTNode> right;

   public:
    HTNode(AlphabetSymbol s, float f,
        std::shared_ptr<HTNode> L, std::shared_ptr<HTNode> R);
    HTNode(std::shared_ptr<HTNode> L, std::shared_ptr<HTNode> R);

    void generateHuffmanTable(HuffmanTable &huffman_table);
    void encodeHuffmanTree(std::ostream &out);

    friend bool operator> (const std::shared_ptr<HTNode> &n1,
                           const std::shared_ptr<HTNode> &n2);
  };

  friend bool operator> (const std::shared_ptr<HTNode> &n1,
                         const std::shared_ptr<HTNode> &n2);

  std::shared_ptr<HTNode> huffman_tree;
  HuffmanTable huffman_table;
  // length of an alphabet symbol in bytes. This Huffman encoder assumes all
  // alphabet symbols are the same length.
  int alphabet_length;
  void finishConstructing(const std::vector<std::shared_ptr<HTNode>> &tree_nodes);

 public:
  HuffmanEncoder(const std::vector<SFEntry> &symbol_frequencies);
  HuffmanEncoder(const char *filename);

  void encode(std::istream &in, std::ostream &out, bool verbose);
  void printHuffmanTable();
};

#endif
