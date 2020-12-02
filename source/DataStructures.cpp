#include "../headers/DataStructures.h"

LinkedNode::LinkedNode(NonLinkedNode nln) : LinkedNode(nln.ch, nln.count, nullptr, nullptr) {}

bool isLeaf(LinkedNode *node)
{
    return !(node->left && node->right);
}