# A Huffman Coder

I had an old Huffman encoder written in C collecting dust in my homedir.
I rewrote it in C++11 (I wanted to learn C++11), made up a file format
and put together a decoder for it.

This is a learning exercise and a toy. Please don't use this code in anything
important. I'm pretty sure this code will kick puppies if you try to encode
and decode on machines with different endianness.

    make
    ./huffman_encoder /usr/share/dict/words words.enc
    ./huffman_decoder words.enc words.dec
    diff /usr/share/dict/words words.dec
