#ifndef COMPILER_CODEGENERATOR_H
#define COMPILER_CODEGENERATOR_H

#include <string>

class CodeSnippet {
public:
    std::string command;
    std::string label;
    std::string firstValue;
    std::string secondValue;
    CodeSnippet *next;

    CodeSnippet(std::string);

    CodeSnippet(int, std::string);

    CodeSnippet(std::string, int);

    CodeSnippet(int, std::string, int);

    CodeSnippet(std::string, int, int);

    CodeSnippet(int, std::string, int, int);

};

class CodeGenerator {
private:
    CodeSnippet *head;
public:
    CodeGenerator();

    void insertNode(CodeSnippet *);

    void printList();

    void generateCodeList();
};


#endif //COMPILER_CODEGENERATOR_H
