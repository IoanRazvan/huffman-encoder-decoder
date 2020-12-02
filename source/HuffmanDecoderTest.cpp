#include "../headers/HuffmanDecoder.h"
#include <stdexcept>
#include <string>

int main(int argc, const char * argv[])
{
    if (argc != 3)
    {
        std::string errorMessage = std::string("Usage: ") + argv[0] + " <input filename> <decoded filename>";
        throw std::logic_error(errorMessage);
    }

    HuffmanDecoder decoder(argv[1]);
    decoder.decodeFile(argv[2]);

    return 0;
}