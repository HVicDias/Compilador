#include "semanticAnalyser.h"

int lineNo;

SymbolTable symbolTable;

bool searchDuplicatedVariableTable(Node token) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchLocalSymbol(token.lexema);

    if (nodeToken == nullptr) {
        return false;
    } else {
        return true;
    }
}

bool searchDeclaratedVariableTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(lexema);

    if (nodeToken == nullptr
        //        || nodeToken->type == "booleano"
        || nodeToken->type == "função inteiro"
        || nodeToken->type == "função booleano"
        || nodeToken->type == "procedimento") {
        return false;
    } else {
        return true;
    }
}

bool searchDeclaratedFunctionTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(lexema);

    if (nodeToken == nullptr
        || nodeToken->type == "inteiro"
        || nodeToken->type == "booleano"
        || nodeToken->type == "procedimento") {
        return false;
    } else {
        return true;
    }
}

bool searchDeclaratedVariableOrFunctionTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(lexema);

    if (nodeToken == nullptr
        || nodeToken->type == "função booleano"
        || nodeToken->type == "procedimento") {
        return false;
    } else {
        return true;
    }
}

bool searchDeclaratedProcedureTable(std::string lexema) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchSymbol(lexema);

    if (nodeToken == nullptr
        || nodeToken->type == "inteiro"
        || nodeToken->type == "booleano"
        || nodeToken->type == "função inteiro"
        || nodeToken->type == "função booleano"
            ) {
        return false;
    } else {
        return true;
    }
}

bool searchDuplicatedProcedureTable(Node token) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchLocalSymbol(token.lexema);

    if (nodeToken == nullptr
        || nodeToken->type == "função booleano"
            ) {
        return false;
    } else {
        return true;
    }
}


