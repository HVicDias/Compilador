#ifndef COMPILER_LINKEDLIST_H
#define COMPILER_LINKEDLIST_H

#include <string>

class Node {
public:
    std::string lexema;
    std::string simbolo;
    Node *next;

    Node(std::string, std::string);

    Node();
};

class LinkedList {
private:
    Node *head;
public:
    LinkedList();

    void insertNode(std::string, std::string);

    void printList();

    void deleteNode(int);
};

#endif //COMPILER_LINKEDLIST_H
