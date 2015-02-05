#include <fstream>
#include <cstring>

#include "HuffmanDecoder.h"

using std::ifstream;
using std::ofstream;
using std::istream;
using std::ostream;
using std::cin;
using std::cout;
using std::cerr;

using std::make_shared;
using std::shared_ptr;

using std::string;
using std::endl;

// get n chars without delimiter
static int getNChars(string &a, int n, istream &in) {
  int i;
  for (i=0; i<n; i++) {
    if (!in.get(a[i])) {
      break;
    }
  }
  return i;
}

HuffmanDecoder::HTNode::HTNode(shared_ptr<HTNode> L, shared_ptr<HTNode> R)
  : left(L), right(R) {}

HuffmanDecoder::HuffmanDecoder(istream &I, ostream &O = cout) {
  in = &I;
  out = &O;
  huffman_tree = make_shared<HTNode>(nullptr, nullptr);
}

void HuffmanDecoder::decode(bool verbose = false) {
  int remainder = -1;
  int alphabet_length = -1;
  string chunk(3, 0);

  if (verbose)
    cout << "Decoding..." << endl;

  while (getNChars(chunk, 3, *in)) {
    if (chunk.compare("ABL") == 0) {
      char c;
      in->get(c);
      alphabet_length = (int) c;
      if (verbose)
        cout << "Alphabet length " << alphabet_length << endl;
    } else if (chunk.compare("HMT") == 0) {
      if (alphabet_length == -1) {
        cerr << "WARNING: alphabet_length not specified. Assuming 1.";
        cerr << endl;
        alphabet_length = 1;
      }
      huffman_tree->decodeHuffmanTree(*in, alphabet_length);
    } else if (chunk.compare("REM") == 0) {
      char c;
      *in >> c;
      remainder = (int) c;
      if (verbose)
        cout << "Remainder " << remainder << endl;
    } else if (chunk.compare("DAT") == 0) {
      if (remainder == -1) {
        cerr << "WARNING: remainder not specified. Assuming 0.";
        cerr << endl;
        remainder = 0;
      }
      if (verbose)
        cout << "Starting to read data." << endl;
      break;
    } else {
      cerr << "WARNING: Don't recognize the chunk [" << chunk << "]";
      cerr << endl;
    }
  }

  huffman_tree->decodeData(*in, *out, huffman_tree, remainder);
}

void HuffmanDecoder::HTNode::decodeHuffmanTree(istream &in,
                                               int alphabet_length) {
  char indicator;
  in.get(indicator);

  bool has_left_child = (1 << 1) & indicator;
  bool has_right_child = (1 << 0) & indicator;

  if (indicator == 0) {  // leaf node
    for (int i=0; i<alphabet_length; i++) {
      char c;
      in.get(c);
      symbol.push_back(c);
    }
  } else {
    if (has_left_child) {
      left = make_shared<HTNode>(nullptr, nullptr);
      left->decodeHuffmanTree(in, alphabet_length);
    }
    if (has_right_child) {
      right = make_shared<HTNode>(nullptr, nullptr);
      right->decodeHuffmanTree(in, alphabet_length);
    }
  }
}

void HuffmanDecoder::HTNode::decodeData(istream &in, ostream &out,
    const shared_ptr<HTNode> &root, int remainder) {
  // TODO what to do with malformed files?
  char c;
  shared_ptr<HTNode> curr = root;
  while (in.get(c)) {
    in.peek();
    int bits = in.eof() ? (BYTE_SIZE - remainder) : BYTE_SIZE;
    for (int i=0; i<bits; i++) {
      bool b = (1 << (BYTE_SIZE-1-i)) & c;
      if (b == 0)
        curr = curr->left;
      else
        curr = curr->right;
      if (curr->left == nullptr && curr->right == nullptr) {
        for (const auto &b : curr->symbol)
          out << b;
        curr = root;
      }
    }  // end for
  }  // end while
}

static void printUsage(const char *name) {
  cout << name << " <infile or --> [<outfile>]" << endl;
  cout << "\tIf -- is given instead of an infile, " << name << " will read "
    "from stdin instead." << endl;
  cout << "\tSpecifying an outfile is optional; if one is not given output ";
  cout << "will be sent to stdout." << endl;
}

int main(int argc, const char *argv[]) {
  if (2 <= argc && argc <= 3) {
    istream *in = &cin;
    ostream *out = &cout;

    if (strcmp("-h", argv[1]) == 0 || strcmp("--help", argv[1]) == 0) {
      printUsage(argv[0]);
      return 0;
    } else if (strcmp("--", argv[1]) != 0) {
      in = new ifstream(argv[1]);
    }

    if (argc == 3)
      out = new ofstream(argv[2]);

    HuffmanDecoder huffman(*in, *out);
    huffman.decode();

    if (in != &cin)
      delete(in);
    if (out != &cout)
      delete(out);
  } else {
    cout << "ERROR: Expected 1 or 2 arguments; got " << argc-1 << "." << endl;
    printUsage(argv[0]);
  }

  return 0;
}
