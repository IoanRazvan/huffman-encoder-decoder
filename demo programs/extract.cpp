#include "..\HuffmanTree.h"


int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "You forgot to provide the two file names!";
        std::exit(EXIT_FAILURE);
    }
    HuffmanTree huff(argv[1], HuffmanTree::Action::EXTRACT);

#ifdef OUTPUT
    std::cout << huff;
#endif
    huff.extractFileTo(argv[2]);

    return 0;
}