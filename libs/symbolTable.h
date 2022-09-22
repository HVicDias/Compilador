#ifndef COMPILER_LINKEDLIST_H
#define COMPILER_LINKEDLIST_H

#include <string>

class SymbolNode {
public:
    std::string identifier;
    std::string scope;
    std::string type;
    int lineNo;
    SymbolNode *next;

    SymbolNode(std::string, std::string, std::string, int);

    SymbolNode();
};

class SymbolList {
private:
    SymbolNode *head;
public:
    SymbolList();

    void insertNode(std::string, std::string, std::string, int);

    void printList();

    void deleteNode(int);
};

class SymbolListNode : public SymbolList{
public:
    SymbolListNode *previous;

    SymbolListNode();
};

class SymbolTable{
private:
    SymbolListNode *head;
public:
    SymbolTable();

    void insertNode(std::string, std::string);

    void printList();

    void deleteNode();
};

#endif //COMPILER_LINKEDLIST_H
