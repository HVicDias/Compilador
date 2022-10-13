#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

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

class SymbolListNode : public SymbolList {
public:
    SymbolListNode *previous;

    std::string layerName;

    SymbolListNode(std::string);
};

class SymbolTable {
private:
    SymbolListNode *head;

public:
    SymbolTable();

    void insertSymbol(std::string, std::string, std::string, int);

    void downLayer(std::string);

    void printList();

    void deleteLayer();
};

#endif //COMPILER_SYMBOLTABLE_H
