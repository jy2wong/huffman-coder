#ifndef HUFFMAN_DECODER_H_
#define HUFFMAN_DECODER_H_

#include <memory>
#include <string>
#include <iostream>

using AlphabetSymbol = std::string;

#define BYTE_SIZE 8


class HuffmanDecoder {
 private:
  std::istream *in;
  std::ostream *out;

  class HTNode {
   private:
    AlphabetSymbol symbol;
    std::shared_ptr<HTNode> left;
    std::shared_ptr<HTNode> right;

   public:
    HTNode(std::shared_ptr<HTNode> L, std::shared_ptr<HTNode> R);
    void decodeHuffmanTree(std::istream &in, int alphabet_length);
    void decodeData(std::istream &in, std::ostream &out,
        const std::shared_ptr<HTNode> &root, int remainder);
  };

  std::shared_ptr<HTNode> huffman_tree;

 public:
  HuffmanDecoder(std::istream &I, std::ostream &O);
  void decode(bool verbose);
};

#endif
