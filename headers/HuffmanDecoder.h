#ifndef HUFFMAN_DECODER_H
#define HUFFMAN_DECODER_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <iostream>
#include <fstream>
#include "DataStructures.h"
#include "StackAllocator.hpp"

class HuffmanDecoder
{
private:
    std::string fileName;
    size_t numberOfNodes;
    StackAllocator<LinkedNode> linkedNodesAllocator;
    LinkedNode * root;


    std::vector<NonLinkedNode> getTree();
    void setMembers(std::vector<NonLinkedNode> & nonLinkedNodes);
    void convertArrayToTree(std::vector<NonLinkedNode> & nodes);
    void convertFileContents(std::ifstream & fin, std::ofstream & fout);
public:
    HuffmanDecoder(const std::string & fileName);
    void decodeFile(const std::string & outputFileName);
};

#endif