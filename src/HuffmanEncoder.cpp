#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <new>
#include <list>
#include <vector>
#include <stack>
#include <cstring>
#include <cerrno>
#include <climits>
#include "HuffmanEncoder.h"
#include "file_stream_handling.h"

using namespace std;

HuffmanEncoder::HuffmanEncoder(const string &fileName) : fileName(fileName), root(nullptr), numberOfLeafNodes(0), numberOfNodes(0)
{
    ifstream fin = openFileInputStream(fileName, ios::in | ios::binary);
    map<char, unsigned long> charToOccurrrences = getCharToOccurrences(fin);
    setMembers(charToOccurrrences);
}

map<char, unsigned long> HuffmanEncoder::getCharToOccurrences(ifstream &fin)
{
    char currentCharacter;
    map<char, unsigned long> charToOccurrences;
    while (fin.read(&currentCharacter, sizeof(char)))
        if (charToOccurrences.find(currentCharacter) != charToOccurrences.end())
            charToOccurrences[currentCharacter]++;
        else
            charToOccurrences.insert({currentCharacter, 1});

    return charToOccurrences;
}

void HuffmanEncoder::setMembers(map<char, unsigned long> &charToOccurrences)
{
    if (charToOccurrences.size() == 0)
        return;
    else if (charToOccurrences.size() == 1)
        charToOccurrences.insert({EOF, ULONG_MAX});
    numberOfLeafNodes = charToOccurrences.size();
    numberOfNodes = 2 * numberOfLeafNodes - 1;
    linkedNodesAllocator = move(StackAllocator<LinkedNode>(numberOfNodes));
    setRoot(charToOccurrences);
    setCharToCode();
}

void HuffmanEncoder::setRoot(map<char, unsigned long> &charToOccurrences)
{
    list<LinkedNode *> leafNodes(numberOfLeafNodes);
    transform(charToOccurrences.begin(), charToOccurrences.end(), leafNodes.begin(),
              [this](auto charOccurrencesPair) { return new (this->linkedNodesAllocator.nextAddress()) LinkedNode(charOccurrencesPair.first, charOccurrencesPair.second); });
    buildTree(leafNodes);
}

void HuffmanEncoder::buildTree(list<LinkedNode *> & leafNodes)
{
    auto comparator = [](const LinkedNode *a, const LinkedNode *b) {
        return a->count < b->count;
    };

    while (leafNodes.size() > 1)
    {
        auto first_minimum_it = min_element(leafNodes.begin(), leafNodes.end(), comparator);
        LinkedNode *first_minimum = *first_minimum_it;
        leafNodes.erase(first_minimum_it);
        auto second_minimum_it = min_element(leafNodes.begin(), leafNodes.end(), comparator);
        LinkedNode *second_minimum = *second_minimum_it;
        leafNodes.erase(second_minimum_it);
        leafNodes.push_front(new (linkedNodesAllocator.nextAddress()) LinkedNode(EOF, first_minimum->count + second_minimum->count, first_minimum, second_minimum));
    }
    root = leafNodes.front();
}


void HuffmanEncoder::setCharToCode()
{
    stack<pair<LinkedNode *, string>> st;

    st.push({root, ""});
    while (!st.empty())
    {
        auto [currentRoot, code] = st.top();
        st.pop();
        if (!isLeaf(currentRoot))
        {
            st.push({currentRoot->left, code + "1"});
            st.push({currentRoot->right, code + "0"});
        }
        else
            charToCode.insert({currentRoot->ch, code});
    }
}

void HuffmanEncoder::encodeFile(const string &encodedFileName)
{
    ofstream fout = openFileOutputStream(encodedFileName, ios::trunc | ios::binary);
    if (root)
    {
        ifstream fin = openFileInputStream(fileName, ios::in | ios::binary);
        vector<NonLinkedNode> fileWriteableTree = convertTreeToArray();
        fout.write(reinterpret_cast<char *>(&numberOfNodes), sizeof(size_t));
        fout.write(reinterpret_cast<char *>(&fileWriteableTree[0]), numberOfNodes * sizeof(NonLinkedNode));
        convertFileContents(fin, fout);
    }
}

vector<NonLinkedNode> HuffmanEncoder::convertTreeToArray()
{
    stack<pair<LinkedNode *, int>> st;
    vector<NonLinkedNode> fileWriteableTree;
    fileWriteableTree.reserve(numberOfNodes);

    st.push({root, -1});
    while (!st.empty())
    {
        auto [currentRoot, parentIndex] = st.top();
        st.pop();
        fileWriteableTree.emplace_back(currentRoot, parentIndex);
        if (!isLeaf(currentRoot))
        {
            st.push({currentRoot->right, fileWriteableTree.size() - 1});
            st.push({currentRoot->left, fileWriteableTree.size() - 1});
        }
    }

    return fileWriteableTree;
}

static char nextState(char conversionCharacter, short whichBit, bool bitValue);

void HuffmanEncoder::convertFileContents(ifstream & fin, ofstream &fout)
{
    ios::pos_type unusedLastByteBitsPos = fout.tellp();
    short lastModifiedBit = 0;
    char currentInputCharacter, conversionCharacter = 0;

    fout.write(reinterpret_cast<char *>(&lastModifiedBit), sizeof(short));
    while (fin.read(&currentInputCharacter, sizeof(char)))
    {
        for (char codeBit : charToCode[currentInputCharacter])
        {
            conversionCharacter = nextState(conversionCharacter, lastModifiedBit, codeBit == '1' ? true : false);
            lastModifiedBit++;
            if (lastModifiedBit == CHAR_BIT)
            {
                fout.write(&conversionCharacter, sizeof(char));
                lastModifiedBit = 0;
            }
        }
    }
    
    if (lastModifiedBit != 0)
    {
        fout.write(reinterpret_cast<char *>(&conversionCharacter), sizeof(char));
        fout.seekp(unusedLastByteBitsPos, fout.beg);
        fout.write(reinterpret_cast<char *>(&lastModifiedBit), sizeof(short));
    }
}

static char nextState(char conversionCharacter, short whichBit, bool bitValue)
{
    if (bitValue)
        return conversionCharacter | (1 << (CHAR_BIT - whichBit - 1));
    else
        return conversionCharacter & ~(1 << (CHAR_BIT - whichBit - 1));
}