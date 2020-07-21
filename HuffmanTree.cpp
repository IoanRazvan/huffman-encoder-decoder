#include "HuffmanTree.h"

#define HUFFMAN_ERROR_MESSAGE(arg) {\
        std::cerr << "Trying to perform extract operation on a type " << arg << " huffman tree!" << std::endl; \
        std::cerr << "Aborting program!" << std::endl; \
        std::exit(EXIT_FAILURE); }\

#define FILE_CHECK(stream, fileName) \
        if (!stream.is_open()) \
        { \
            std::cerr << "Unable to open " << fileName << std::endl; \
            std::cerr << "Aborting program!"; \
            std::exit(EXIT_FAILURE); \
        } \


HuffmanTree::LinkedNode::LinkedNode(const NonLinkedNode &n)
    : m_ch(n.ch), m_freq(n.freq), m_right(nullptr), m_left(nullptr)
{
}

std::list<HuffmanTree::LinkedNode *> HuffmanTree::charFreq(std::ifstream &is)
{
    std::map<char, int> characterOcurances;
    char currentChar;
    int numberOfCharacters = 0;

    while (is.get(currentChar))
    {
        if (characterOcurances.find(currentChar) == characterOcurances.end())
            characterOcurances.insert(std::pair<char, int>(currentChar, 1));
        else
            characterOcurances[currentChar]++;
        numberOfCharacters++;
    }

    std::list<LinkedNode *> characterFrequencyes;
    for (auto it : characterOcurances)
        characterFrequencyes.push_front(new LinkedNode(it.first, static_cast<double>(it.second) / numberOfCharacters, nullptr, nullptr));

    return characterFrequencyes;
}

HuffmanTree::HuffmanTree(const std::string &inFile, Action operationType)
{
    this->operationType = operationType;
    this->fileName = inFile;
    this->root = nullptr;
}

void HuffmanTree::buildTreeHelper(std::ifstream & fin) {
    std::list<LinkedNode *> characterFrequencies = charFreq(fin);

    while (characterFrequencies.size() > 1)
    {
        auto smallP = getSmallestPair(characterFrequencies);
        LinkedNode *joinedNodes = new LinkedNode(EOF, (*smallP.first)->m_freq + (*smallP.second)->m_freq, *smallP.second, *smallP.first);
        characterFrequencies.erase(smallP.first);
        characterFrequencies.erase(smallP.second);
        characterFrequencies.push_front(joinedNodes);
    }
    root = characterFrequencies.front();
}

void HuffmanTree::buildTree()
{
    if (root != nullptr)
        return;
    std::ifstream fin(fileName);
    FILE_CHECK(fin, fileName);
    buildTreeHelper(fin);
    fin.close();
}

void HuffmanTree::buildTree(std::ifstream & fin) {
    if (root != nullptr)
        return;
    buildTreeHelper(fin);
}

std::pair<std::list<HuffmanTree::LinkedNode *>::iterator, std::list<HuffmanTree::LinkedNode *>::iterator> HuffmanTree::getSmallestPair(std::list<LinkedNode *> &charFreq)
{
    auto small1 = charFreq.begin();
    auto small2 = charFreq.begin();
    std::advance(small2, 1);
    if ((*small1)->m_freq > (*small2)->m_freq)
    {
        auto temp = small1;
        small1 = small2;
        small2 = temp;
    }

    auto i = charFreq.begin();
    std::advance(i, 2);

    while (i != charFreq.end())
    {
        if ((*i)->m_freq < (*small1)->m_freq)
        {
            small2 = small1;
            small1 = i;
        }
        else if ((*i)->m_freq < (*small2)->m_freq)
        {
            small2 = i;
        }
        i++;
    }

    return std::pair<std::list<LinkedNode *>::iterator, std::list<LinkedNode *>::iterator>(small1, small2);
}

void HuffmanTree::getCharacterCodes()
{
    if (operationType == Action::COMPRESS)
        buildTree();
    else
        restoreTree();
    getCharacterCodesHelper(root, "");
}

void HuffmanTree::getCharacterCodesHelper(LinkedNode *root, std::string s)
{
    if (root->m_ch != EOF)
        codes.insert(std::pair<char, std::string>(root->m_ch, s));
    else
    {
        getCharacterCodesHelper(root->m_left, s + "1");
        getCharacterCodesHelper(root->m_right, s + "0");
    }
}

std::vector<HuffmanTree::NonLinkedNode> HuffmanTree::buildArrayVersionOfTree()
{
    std::vector<NonLinkedNode> v;
    buildArrayVersionOfTreeHelper(v, root, -1);
    return v;
}

void HuffmanTree::buildArrayVersionOfTreeHelper(std::vector<NonLinkedNode> &v, LinkedNode *root, int parentIndex)
{
    if (root != nullptr)
    {
        v.push_back(NonLinkedNode(root, parentIndex));
        int currentIndex = v.size() - 1;
        buildArrayVersionOfTreeHelper(v, root->m_left, currentIndex);
        buildArrayVersionOfTreeHelper(v, root->m_right, currentIndex);
    }
}

void HuffmanTree::buildTreeVersionOfArray(std::vector<NonLinkedNode> &arrayVersionOfTree)
{
    int sizeOfArrayTree = arrayVersionOfTree.size();
    std::vector<LinkedNode *> pointersToNodes(sizeOfArrayTree);
    for (int i = 0; i < sizeOfArrayTree; i++)
        pointersToNodes[i] = new LinkedNode(arrayVersionOfTree[i]);

    root = pointersToNodes[0];
    for (int i = 1; i < sizeOfArrayTree; i++)
    {
        if (pointersToNodes[arrayVersionOfTree[i].parent_index]->m_left == nullptr)
            pointersToNodes[arrayVersionOfTree[i].parent_index]->m_left = pointersToNodes[i];
        else
            pointersToNodes[arrayVersionOfTree[i].parent_index]->m_right = pointersToNodes[i];
    }
}

void HuffmanTree::storeTree(std::ofstream &fout)
{
    auto v = buildArrayVersionOfTree();
    size_t s = v.size();
    fout.write(reinterpret_cast<char *>(&s), sizeof(size_t));
    fout.write(reinterpret_cast<char *>(&v[0]), sizeof(NonLinkedNode) * s);
}


void HuffmanTree::restoreTree(std::ifstream &fin)
{
    restoreTreeHelper(fin);
}

void HuffmanTree::restoreTree() 
{
    if (root != nullptr)
        return;
    std::ifstream fin(fileName, std::ios::binary | std::ios::in);
    FILE_CHECK(fin, fileName);
    restoreTreeHelper(fin);
    fin.close();
}

void HuffmanTree::restoreTreeHelper(std::ifstream &fin) 
{
    size_t v_size;
    fin.read(reinterpret_cast<char *>(&v_size), sizeof(size_t));
    std::vector<NonLinkedNode> v(v_size);
    fin.read(reinterpret_cast<char *>(&v[0]), v_size * sizeof(NonLinkedNode));
    buildTreeVersionOfArray(v);
}

HuffmanTree::~HuffmanTree() {
    destructorHelper(root);
}

void HuffmanTree::destructorHelper(LinkedNode *root)
{
    if (root != nullptr)
    {
        destructorHelper(root->m_left);
        destructorHelper(root->m_right);
        delete root;
    }
}

std::string HuffmanTree::operator[](char ch)
{
    if (codes.size() == 0)
        getCharacterCodes();
    if (codes.find(ch) != codes.end())
        return codes[ch];
    else
        return "";
}

std::ostream & operator<<(std::ostream & os, HuffmanTree & h) {
    if (h.codes.size() == 0)
        h.getCharacterCodes();
    for (auto i : h.codes)
        std::cout << i.first << ": " << i.second << std::endl;
    return os;
}

void HuffmanTree::compressFileTo(const std::string &outFile)
{
    if (operationType != Action::COMPRESS)
        HUFFMAN_ERROR_MESSAGE("COMPRESS");

    std::ofstream fout(outFile, std::ios::out | std::ios::binary);
    std::ifstream fin(fileName);

    FILE_CHECK(fin, fileName);
    FILE_CHECK(fout, outFile);

    if (root == nullptr) {
        buildTree(fin);
        fin.clear();
        fin.seekg(0, fin.beg);
    }

    if (codes.size() == 0)
        getCharacterCodes();
    storeTree(fout);
    convertBinary(fin, fout);
}

void HuffmanTree::convertBinary(std::ifstream&fin, std::ofstream &fout)
{
    char currentCharacter;
    char converterCharacter;

    std::ios::pos_type maskPosition = fout.tellp();
    char lastSignificantBits = static_cast<char>(255);
    fout.write(&lastSignificantBits, sizeof(char));

    int converterCharacterIndex = 0;

    while (fin.get(currentCharacter))
    {
        for (int i = 0; i < codes[currentCharacter].size(); i++)
        {
            if (codes[currentCharacter][i] == '1')
                converterCharacter = converterCharacter | 1 << (7 - converterCharacterIndex);
            else
                converterCharacter = converterCharacter & ~1 << (7 - converterCharacterIndex);
            converterCharacterIndex++;
            if (converterCharacterIndex == 8)
            {
                fout.write(&converterCharacter, sizeof(char));
                converterCharacterIndex = 0;
            }
        }
    }

    if (converterCharacterIndex != 0) {
        fout.write(&converterCharacter, sizeof(char));
        lastSignificantBits = lastSignificantBits & (~0 << (7 - converterCharacterIndex));
    }

    fout.seekp(maskPosition, fout.beg);
    fout.write(&lastSignificantBits, sizeof(char));

    fout.close();
    fin.close();
}

void HuffmanTree::extractFileTo(const std::string &outFile)
{
    if (operationType != Action::EXTRACT) 
        HUFFMAN_ERROR_MESSAGE("EXTRACT");

    std::ofstream fout(outFile);
    std::ifstream fin(fileName, std::ios::binary | std::ios::in);

    FILE_CHECK(fout, outFile);
    FILE_CHECK(fin, fileName);

    if (root == nullptr)
        restoreTree(fin);
    else {
        size_t treeSize;
        fin.read(reinterpret_cast<char*>(&treeSize), sizeof(size_t));
        fin.seekg(treeSize * sizeof(NonLinkedNode), fin.cur);
    }
    convertDecimal(fin, fout);
}

void HuffmanTree::convertDecimal(std::ifstream&fin, std::ofstream &fout)
{
    char lastSignificantBits;
    fin.read(&lastSignificantBits, sizeof(char));

    unsigned short currentBitInCharacter = 0;
    char currentCharacter;

    fin.read(reinterpret_cast<char *>(&currentCharacter), sizeof(char));
    LinkedNode *p = root;
    while (true)
    {
        if (currentCharacter & 1 << (7 - currentBitInCharacter))
            p = p->m_left;
        else
            p = p->m_right;

        if (p->m_ch != EOF)
        {
            if (fin.peek() == EOF && !(static_cast<char>(lastSignificantBits & (1 << (7-currentBitInCharacter)))))
                    break;
            fout << p->m_ch;
            p = root; 
        }

        currentBitInCharacter++;
        if (currentBitInCharacter == 8)
        {
            currentBitInCharacter = 0;
            if (!fin.read(reinterpret_cast<char *>(&currentCharacter), sizeof(char)))
                break;
        }
    }

    fout.close();
    fin.close();
}