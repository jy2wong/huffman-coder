#include <queue>
#include <fstream>

#include "HuffmanEncoder.h"

using std::shared_ptr;
using std::make_shared;

using std::greater;

using std::priority_queue;
using std::vector;
using std::string;

using std::istream;
using std::ifstream;
using std::ostream;
using std::cout;
using std::cerr;
using std::endl;

// get n chars without delimiter
static int getNChars(string &a, int n, istream &in) {
  int i;
  for (i=0; i<n; i++) {
    if (!in.get(a[i]))
      break;
  }
  return i;
}

bool operator> (const shared_ptr<HuffmanEncoder::HTNode> &n1, const shared_ptr<HuffmanEncoder::HTNode> &n2) {
  return n1->freq > n2->freq;
}

HuffmanEncoder::HTNode::HTNode(AlphabetSymbol s, float f,
    shared_ptr<HTNode> L, shared_ptr<HTNode> R)
  : symbol(s), freq(f), left(L), right(R) {}

HuffmanEncoder::HTNode::HTNode(shared_ptr<HTNode> L, shared_ptr<HTNode> R)
    : left(L), right(R), freq(L->freq + R->freq) {}

HuffmanEncoder::HuffmanEncoder(const vector<SFEntry> &symbol_frequencies) {
  vector<shared_ptr<HTNode>> tree_nodes;
  alphabet_length = symbol_frequencies[0].symbol.length();

  for (const auto &sym : symbol_frequencies) {
    if (sym.freq > 0.0f) {
      auto node = make_shared<HTNode>(sym.symbol, sym.freq, nullptr, nullptr);
      tree_nodes.push_back(node);
    }
  }

  finishConstructing(tree_nodes);
}

// generate symbol frequencies from the given file.
HuffmanEncoder::HuffmanEncoder(const char *filename) {
  vector<shared_ptr<HTNode>> tree_nodes;
  alphabet_length = 1;
  ifstream fin(filename);

  vector<float> symbol_popularity(256, 0.0f);
  char c;
  while (fin.get(c)) {
    symbol_popularity[(unsigned char) c] += 1.0f;
  }


  for (int i=0; i<256; i++) {
    const auto freq = symbol_popularity[i];
    if (freq > 0.0f) {
      auto node = make_shared<HTNode>(string(1, i), freq, nullptr, nullptr);
      tree_nodes.push_back(node);
    }
  }
  finishConstructing(tree_nodes);
  return;
}

void HuffmanEncoder::finishConstructing(
    const vector<shared_ptr<HTNode>> &tree_nodes) {

  priority_queue<shared_ptr<HTNode>,
                 vector<shared_ptr<HTNode>>,
                 greater<shared_ptr<HTNode>>> pq(tree_nodes.begin(),
                                                 tree_nodes.end());

  for (int L=tree_nodes.size(); L>1; --L) {
    auto A = pq.top();
    pq.pop();
    auto B = pq.top();
    pq.pop();
    auto internal = make_shared<HTNode>(A, B);
    pq.push(internal);
  }

  huffman_tree = pq.top();
  huffman_tree->generateHuffmanTable(huffman_table);
  return;
}

void HuffmanEncoder::HTNode::generateHuffmanTable(HuffmanTable &huffman_table) {
  static EncodedSymbol trail;

  // at a terminal node. Read trail and insert table entry
  if (this->left == nullptr && this->right == nullptr)
    huffman_table[this->symbol] = trail;
  else {
    // descend down left branch
    if (this->left != nullptr) {
      trail.push_back(false);
      this->left->generateHuffmanTable(huffman_table);
      trail.pop_back();
    }
    // descend down right branch
    if (this->right != nullptr) {
      trail.push_back(true);
      this->right->generateHuffmanTable(huffman_table);
      trail.pop_back();
    }
  }
}

void HuffmanEncoder::encode(istream &in, ostream &out, bool verbose = false) {
  EncodedSymbol encoded_msg;
  AlphabetSymbol a(alphabet_length, 0);
  int count = 0;
  int acquired = 0;

  while ((acquired = getNChars(a, alphabet_length, in))) {
    if (acquired == alphabet_length) {
      for (const auto &bit : huffman_table[a]) {
        encoded_msg.push_back(bit);
      }
    } else {  // TODO
      for (int i=acquired; i<a.size(); i++) {
        a[i] = 0;
      }
      cerr << "WARNING: input size in bytes is not a multiple of "
              "the alphabet symbol length. "
              "Ignoring last " << count << " bytes." << endl;
    }
    count += acquired;
  }

  // pad with zeros to the next round byte
  char remainder = BYTE_SIZE - (encoded_msg.size() % BYTE_SIZE);
  if (remainder) {
    for (int b=0; b < remainder; b++) {
      encoded_msg.push_back(false);
    }
  }
  if (verbose)
    cout<< "padding with " << (int) remainder << " zeroes." << endl;

  out << "ABL" << (char) alphabet_length;  // AlphaBet Length

  // write huffman tree to stream
  out << "HMT";  // HuffMan Tree
  huffman_tree->encodeHuffmanTree(out);

  out << "REM" << remainder;

  out << "DAT";
  int start = 0;
  while ((encoded_msg.size() - start) >= BYTE_SIZE) {
    char c = 0;
    for (int b=0; b<BYTE_SIZE; b++) {
      if (verbose)
        cout << encoded_msg[start+b];
      c = (c << 1) + encoded_msg[start + b];
    }
    if (verbose)
      cout << ' ';
    out << c;
    start += BYTE_SIZE;
  }

  if (verbose)
    cout << endl;
  return;
}

// depth first traversal
void HuffmanEncoder::HTNode::encodeHuffmanTree(ostream &out) {
  bool has_left_child = !!(left != nullptr);
  bool has_right_child = !!(right != nullptr);

  char indicator = (has_left_child << 1) | has_right_child;
  out << indicator;

  if (indicator == 0) {  // leaf node
    for (const auto &c : symbol)
      out << c;
  }

  if (has_left_child)
    left->encodeHuffmanTree(out);
  if (has_right_child)
    right->encodeHuffmanTree(out);
}

void HuffmanEncoder::printHuffmanTable() {
  cout << "[AlphabetSymbol] -> EncodedSymbol mapping:" << endl;
  for (const auto &kv : huffman_table) {
    cout << "[" << kv.first << "] -> ";
    for (const auto &b : kv.second) {
      cout << b;
    }
    cout << endl;
  }
}
