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
public:
    SymbolNode *symbolNode;

    SymbolList();

    void insertNode(std::string, std::string, std::string, int);

    void printList();

    void deleteNode(int);
};

class SymbolListNode : public SymbolList {
public:
    SymbolListNode *previous;

    std::string layerName;

    explicit SymbolListNode(std::string);
};

class SymbolTable {
private:
    SymbolListNode *symbolListNode;

public:
    SymbolTable();

    void insertSymbol(std::string, std::string, std::string, int);

    void downLayer(std::string);

    SymbolNode* searchSymbol(std::string);

    void printList();

    void deleteLayer();
};

#endif //COMPILER_SYMBOLTABLE_H
