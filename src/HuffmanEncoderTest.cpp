#include "HuffmanEncoder.h"
#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, const char * argv[])
{
    if (argc != 3) 
    {
        std::string errorMessage = std::string("Usage: ") + argv[0] + " <input filename> <encoded filename>";
        throw std::logic_error(errorMessage);
    }

    HuffmanEncoder encoder(argv[1]);
    std::cout << "Character codes:\n";
    for (auto & [character, code] : encoder)
        std::cout << character << " " << code << std::endl;
    encoder.encodeFile(argv[2]);
    
    return 0;
}