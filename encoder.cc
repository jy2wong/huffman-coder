#include <iostream>
#include <fstream>
#include <cstring>

#include "HuffmanEncoder.h"

using namespace std;

static void printUsage(const char *name) {
  cout << name << " file_to_encode.txt [output.txt or --]" << endl;
}

int main(int argc, const char *argv[]) {
  if (argc == 2 || argc == 3) {
    if (strcmp("-h", argv[1]) == 0 || strcmp("--help", argv[1]) == 0) {
      printUsage(argv[0]);
      return 0;
    }

    ifstream fin(argv[1]);
    HuffmanEncoder h(argv[1]);

    if (argc == 2 || (argc == 3 && strcmp("--", argv[2]) == 0))
      h.encode(fin, cout, false);
    else {
      ofstream fout(argv[2]);
      h.encode(fin, fout, false);
    }
  } else {
    cout << "Expected 1 or 2 arguments; got " << argc-1 << "." << endl;
    printUsage(argv[0]);
  }

  return 0;
}
