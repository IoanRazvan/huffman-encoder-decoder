#include "..\HuffmanTree.h"


int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "You forgot to provide the two file names!";
        std::exit(EXIT_FAILURE);
    }
    HuffmanTree huff(argv[1], HuffmanTree::Action::COMPRESS);

#ifdef OUTPUT
    std::cout << huff;
#endif
    huff.compressFileTo(argv[2]);

    return 0;
}