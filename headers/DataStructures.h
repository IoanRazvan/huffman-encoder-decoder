#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

struct NonLinkedNode;

struct LinkedNode {
    char ch;
    unsigned long count;
    LinkedNode * right;
    LinkedNode * left;
    LinkedNode(char c, unsigned long count, LinkedNode *  l, LinkedNode * r) : ch(c), count(count), left(l), right(r) {}
    LinkedNode(char c, unsigned long count) : LinkedNode(c, count, nullptr, nullptr) {}
    LinkedNode(NonLinkedNode nln);
};

struct NonLinkedNode {
    char ch;
    int parentIndex;
    unsigned long count;
    NonLinkedNode(const LinkedNode * ln, int parentIndex) : ch(ln->ch), count(ln->count), parentIndex(parentIndex) {}
    NonLinkedNode() = default;
};

bool isLeaf(LinkedNode * node);

#endif