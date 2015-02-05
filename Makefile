CC=clang++
CFLAGS=-std=c++11 -g
.PHONY=all clean

all: encode_demo huffman_decoder huffman_encoder

huffman_decoder: HuffmanDecoder.cc HuffmanDecoder.h
	$(CC) $(CFLAGS) $< -o $@

huffman_encoder: encoder.cc HuffmanEncoder.o
	$(CC) $(CFLAGS) $^ -o $@

encode_demo: demo.cc HuffmanEncoder.o
	$(CC) $(CFLAGS) $^ -o $@

HuffmanEncoder.o : HuffmanEncoder.cc HuffmanEncoder.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f huffman_encoder huffman_decoder encode_demo *.o demo/*.out
