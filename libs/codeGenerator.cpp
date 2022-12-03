#include <string>
#include <iostream>
#include <iostream>
#include <fstream>
#include <utility>
#include "codeGenerator.h"

using namespace std;

CodeSnippet::CodeSnippet(std::string command) {
    this->command = command;
    label = "";
    firstValue = "";
    secondValue = "";
    commentary = "";
}

CodeSnippet::CodeSnippet(int label, std::string command) {
    this->command = command;
    this->label = to_string(label);
    firstValue = "";
    secondValue = "";
    commentary = "";
}

CodeSnippet::CodeSnippet(std::string command, int firstValue) {
    this->command = command;
    label = "";
    this->firstValue = to_string(firstValue);
    secondValue = "";
    commentary = "";
}

CodeSnippet::CodeSnippet(std::string command, std::string firstValue) {
    this->command = command;
    label = "";
    this->firstValue = firstValue;
    secondValue = "";
    commentary = "";
}

CodeSnippet::CodeSnippet(int label, std::string command, int firstValue) {
    this->command = command;
    this->label = to_string(label);
    this->firstValue = to_string(firstValue);
    secondValue = "";
    commentary = "";
}

CodeSnippet::CodeSnippet(std::string command, int firstValue, int secondValue) {
    this->command = command;
    label = "";
    this->firstValue = to_string(firstValue);
    this->secondValue = to_string(secondValue);
    commentary = "";
}

CodeSnippet::CodeSnippet(int label, std::string command, int firstValue, int secondValue) {
    this->command = command;
    this->label = to_string(label);
    this->firstValue = to_string(firstValue);
    this->secondValue = to_string(secondValue);
    commentary = "";
}

CodeGenerator::CodeGenerator() { head = nullptr; }

void CodeGenerator::insertNode(CodeSnippet *code) {
    code->next = nullptr;
    if (head == nullptr) {
        head = code;
        return;
    }

    CodeSnippet *auxValue = head;
    while (auxValue->next != nullptr) {
        auxValue = auxValue->next;
    }

    // Insert at the last.
    auxValue->next = code;
}

void CodeGenerator::printList() {
    CodeSnippet *auxNode = head;

    // Check for empty list.
    if (head == nullptr) {
        cout << "List empty" << endl;
        return;
    }

    cout << "label" << "\tcommand" << "\tfirst value" << "\tsecond value" << endl;
    while (auxNode != nullptr) {
        cout << auxNode->label << " : " << auxNode->command << " : " << auxNode->firstValue << " : "
             << auxNode->secondValue << "\n";
        auxNode = auxNode->next;
    }
    cout << endl;
}

void CodeGenerator::generateCode() {
    ofstream MyFile("teste.obj");
    string snippet;
    CodeSnippet *auxNode = head;

    // Check for empty list.
    if (head == nullptr) {
        MyFile << "" << endl;
        return;
    }

    while (auxNode != nullptr) {
        snippet.append(addSpaces(auxNode->label, 4));
        snippet.append(addSpaces(auxNode->command, 8));
        snippet.append(addSpaces(auxNode->firstValue, 4));
        snippet.append(addSpaces(auxNode->secondValue, 4));
        if (auxNode->command != "HLT")
            snippet.append("\n\0");
        MyFile << snippet;
        auxNode = auxNode->next;
        snippet = "";
    }
    MyFile << endl;
    MyFile.close();
}

std::string CodeGenerator::addSpaces(std::string myString, int expectedSize) {
    std::string aux;
    for (int i = myString.length(); i < expectedSize; i++) {
        aux.append(" ");
    }
    myString.append(aux);
    return myString;
}


std::list<CodeSnippet> CodeGenerator::deleteCode() {
    CodeSnippet *auxNode;
    list<CodeSnippet> codeList;
    while (head != nullptr) {
        auxNode = head;
        head = head->next;
        codeList.push_back(*auxNode);
        delete auxNode;
    }
    return codeList;
}
