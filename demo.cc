#include "HuffmanEncoder.h"

#include <vector>
#include <fstream>

using std::ifstream;
using std::ofstream;
using std::cout;
using std::cerr;
using std::endl;
using std::vector;

int main(int argc, const char* argv[]) {
  // https://en.wikipedia.org/wiki/Letter_frequency
  vector<SFEntry> english_alphabet = {
    {"a", 0.08167},
    {"b", 0.01492},
    {"c", 0.02782},
    {"d", 0.04253},
    {"e", 0.12702},
    {"f", 0.02228},
    {"g", 0.02015},
    {"h", 0.06094},
    {"i", 0.06966},
    {"j", 0.00153},
    {"k", 0.00772},
    {"l", 0.04025},
    {"m", 0.02406},
    {"n", 0.06749},
    {"o", 0.07507},
    {"p", 0.01929},
    {"q", 0.00095},
    {"r", 0.05987},
    {"s", 0.06327},
    {"t", 0.09059},
    {"u", 0.02758},
    {"v", 0.00978},
    {"w", 0.02360},
    {"x", 0.00150},
    {"y", 0.01974},
    {"z", 0.00074}
    };

  ifstream fin0("demo/alpha.in");
  ofstream fout0("demo/alpha.out");
  HuffmanEncoder h_alpha(english_alphabet);
  h_alpha.printHuffmanTable();
  h_alpha.encode(fin0, fout0, true);

  cout << "------------------------------" << endl;

  ifstream fin1("demo/text1.in");
  ofstream fout1("demo/text1.out");

  HuffmanEncoder h1("demo/text1.in");
  h1.printHuffmanTable();
  h1.encode(fin1, fout1, true);

  cout << "------------------------------" << endl;

  return 0;
}
