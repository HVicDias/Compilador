#include <iostream>
#include "semanticAnalyser.h"

int lineNo;

SymbolTable symbolTable;

bool searchDuplicatedVariableTable(Node token) {
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchLocalSymbol(token.lexema);

    if(nodeToken == nullptr){
        return false;
    }else{
        return true;
    }
}

bool searchDeclaratedVariableTable(Node token) {
    //analisa_leia
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchLocalSymbol(token.lexema);

    if(nodeToken == nullptr){
        return false;
    }else{
        return true;
    }
}

bool searchDeclaratedFunctionTable(Node token) {
    //analisa_escreva e analisa_declaração_função
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchLocalSymbol(token.lexema);

    if(nodeToken == nullptr){
        return false;
    }else{
        return true;
    }
}

bool searchDuplicatedProcedureTable(Node token) {
    //analisa_declaração_procedimento
    SymbolNode *nodeToken;

    nodeToken = symbolTable.searchLocalSymbol(token.lexema);

    if(nodeToken == nullptr){
        return false;
    }else{
        return true;
    }
}


