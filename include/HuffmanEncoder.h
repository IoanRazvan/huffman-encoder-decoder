#ifndef HUFFMAN_ENCODER_H
#define HUFfMAN_ENCODER_H

#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <list>
#include "StackAllocator.hpp"
#include "DataStructures.h"

class HuffmanEncoder
{
private:
    std::string fileName;
    std::map<char, std::string> charToCode;
    size_t numberOfNodes;
    size_t numberOfLeafNodes;
    StackAllocator<LinkedNode> linkedNodesAllocator;
    LinkedNode * root;

    std::map<char, unsigned long> getCharToOccurrences(std::ifstream & fin);
    void setMembers(std::map<char, unsigned long> & charToOccurrences);
    void setRoot(std::map<char, unsigned long> & charToOccurrences);
    void buildTree(std::list<LinkedNode *> &leafNodes);
    void setCharToCode();
    std::vector<NonLinkedNode> convertTreeToArray();
    void convertFileContents(std::ifstream & fin, std::ofstream & fout);
public:
    HuffmanEncoder(const std::string & fileName);
    void encodeFile(const std::string & outputFileName);
    typedef std::map<char, std::string>::iterator iterator;
    iterator begin() { return charToCode.begin(); }
    iterator end() { return charToCode.end(); }
    std::string operator[](char c) { return charToCode.operator[](c); }
};

#endif