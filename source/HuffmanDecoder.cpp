#include <algorithm>
#include <stdexcept>
#include <new>
#include <climits>
#include "../headers/HuffmanDecoder.h"
#include "../headers/file_stream_handling.h"

using namespace std;

HuffmanDecoder::HuffmanDecoder(const string &fileName) : fileName(fileName), numberOfNodes(0), root(nullptr)
{
    vector<NonLinkedNode> nonLinkedNodes = getTree();
    setMembers(nonLinkedNodes);
}

void HuffmanDecoder::setMembers(vector<NonLinkedNode> &nonLinkedNodes)
{
    if (numberOfNodes)
    {
        linkedNodesAllocator = move(StackAllocator<LinkedNode>(numberOfNodes));
        convertArrayToTree(nonLinkedNodes);
    }
}

vector<NonLinkedNode> HuffmanDecoder::getTree()
{
    ifstream fin = openFileInputStream(fileName, ios::in | ios::binary);

    fin.read(reinterpret_cast<char *>(&numberOfNodes), sizeof(size_t));
    vector<NonLinkedNode> nodes(numberOfNodes);
    fin.read(reinterpret_cast<char *>(&nodes[0]), numberOfNodes * sizeof(NonLinkedNode));
    // for (int i = 0; i < nodes.size(); i++)
    //     cout << nodes[i].ch << " " << nodes[i].parentIndex << "; ";
    // cout << endl;
    return nodes;
}

void HuffmanDecoder::convertArrayToTree(vector<NonLinkedNode> &nodes)
{
    vector<LinkedNode *> linkedNodes(nodes.size());

    transform(nodes.begin(), nodes.end(), linkedNodes.begin(),
              [this](auto node) { return new (this->linkedNodesAllocator.nextAddress()) LinkedNode(node); });

    for (size_t i = 1; i < nodes.size(); i++)
        if (linkedNodes[nodes[i].parentIndex]->left)
            linkedNodes[nodes[i].parentIndex]->right = linkedNodes[i];
        else
            linkedNodes[nodes[i].parentIndex]->left = linkedNodes[i];
    root = linkedNodes[0];
}

void HuffmanDecoder::decodeFile(const string &outputFileName)
{
    ifstream fin = openFileInputStream(fileName, ios::in | ios::binary);
    ofstream fout = openFileOutputStream(outputFileName);
    if (numberOfNodes)
    {
        fin.seekg((numberOfNodes) * sizeof(NonLinkedNode) + sizeof(size_t));
        convertFileContents(fin, fout);
    }
}

static LinkedNode * nextRoot(LinkedNode * currentNode, char currentCharacter, short currentBit);

void HuffmanDecoder::convertFileContents(ifstream &fin, ofstream &fout)
{
    short unusedBits;
    char currentCharacter;
    LinkedNode *dynamicRoot = root;
    
    fin.read(reinterpret_cast<char *>(&unusedBits), sizeof(short));
    unusedBits = CHAR_BIT - unusedBits;

    while (fin.read(&currentCharacter, sizeof(char)))
    {
        for (short currentBit = CHAR_BIT - 1; currentBit >= 0; currentBit--)
        {
            dynamicRoot = nextRoot(dynamicRoot, currentCharacter, currentBit);
            if (isLeaf(dynamicRoot))
            {
                fout.write(&dynamicRoot->ch, sizeof(char));
                dynamicRoot = root;
                if (fin.peek() == EOF && currentBit >= unusedBits)
                    break;
            }
        }
    }
}

LinkedNode * nextRoot(LinkedNode * currentNode, char currentCharacter, short currentBit)
{
    if (currentCharacter & (1 << currentBit))
        return currentNode->left;
    else
        return currentNode->right;
}