#ifndef HUFFMAN_TREE_H
#define HUFFMAN_TREE_H

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <utility>

class HuffmanTree
{
public:
    enum class Action
    {
        COMPRESS,
        EXTRACT
    };

private:
    struct NonLinkedNode;

    struct LinkedNode
    {
        char m_ch;
        double m_freq;
        LinkedNode *m_right;
        LinkedNode *m_left;

        LinkedNode(char ch, double freq, LinkedNode *right, LinkedNode *left) : m_ch(ch), m_freq(freq), m_right(right), m_left(left) {}
        LinkedNode(const NonLinkedNode &n);
    };

    struct NonLinkedNode
    {
        int parent_index;
        char ch;
        double freq;

        NonLinkedNode(LinkedNode *l, int parent_index) : ch(l->m_ch), freq(l->m_freq), parent_index(parent_index) {}
        NonLinkedNode() : parent_index(-1), ch(EOF), freq(0.0f) {}
    };

    Action operationType;
    std::string fileName;
    std::map<char, std::string> codes;
    LinkedNode *root;

    std::list<LinkedNode *> charFreq(std::ifstream &is);
    std::pair<std::list<LinkedNode *>::iterator, std::list<LinkedNode *>::iterator> getSmallestPair(std::list<LinkedNode *> &charFreq);
    
    void getCharacterCodes();
    void getCharacterCodesHelper(LinkedNode *root, std::string code);
    
    std::vector<NonLinkedNode> buildArrayVersionOfTree();
    void buildArrayVersionOfTreeHelper(std::vector<NonLinkedNode> &vec, LinkedNode *root, int parentIndex);
    
    void buildTreeVersionOfArray(std::vector<NonLinkedNode> &arrayVersionOfTree);
    
    void destructorHelper(LinkedNode *root);
    
    void convertBinary(std::ifstream &fin, std::ofstream &fout);
    void convertDecimal(std::ifstream &fin, std::ofstream &fout);

    void restoreTree(std::ifstream &fin);
    void restoreTreeHelper(std::ifstream &fin);

    void buildTree(std::ifstream &fin);
    void buildTreeHelper(std::ifstream &fin);
    
    void storeTree(std::ofstream &fout);
public:
    friend std::ostream & operator<<(std::ostream & os, HuffmanTree & h);
    std::string operator[](char character);
    void buildTree();
    void restoreTree();
    HuffmanTree(const std::string &inFile, Action operationType);
    ~HuffmanTree();
    void extractFileTo(const std::string &outFile);
    void compressFileTo(const std::string &outFile);
};

#endif