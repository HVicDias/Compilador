#include "semanticAnalyser.h"
#include "stack"

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

int precedence(char c) {
    if (c == '^')
        return 3;
    else if (c == '/' || c == '*')
        return 2;
    else if (c == '+' || c == '-')
        return 1;
    else
        return -1;
}

std::string toPostfix(std::string expression) {
    std::stack<char> st; // For stack operations, we are using
    // C++ built in stack
    std::string result;

    for (int i = 0; i < expression.length(); i++) {
        char c = expression[i];

        // If the scanned character is
        // an operand, add it to output string.
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
            || (c >= '0' && c <= '9'))
            result += c;

            // If the scanned character is an
            // ‘(‘, push it to the stack.
        else if (c == '(')
            st.push('(');

            // If the scanned character is an ‘)’,
            // pop and to output string from the stack
            // until an ‘(‘ is encountered.
        else if (c == ')') {
            while (st.top() != '(') {
                result += st.top();
                st.pop();
            }
            st.pop();
        }

            // If an operator is scanned
        else {
            while (!st.empty()
                   && precedence(expression[i]) <= precedence(st.top())) {
                result += st.top();
                st.pop();
            }
            st.push(c);
        }
    }

    // Pop all the remaining elements from the stack
    while (!st.empty()) {
        result += st.top();
        st.pop();
    }

    return result;
}


