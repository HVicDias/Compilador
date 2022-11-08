#include <string>
#include <iostream>
#include <utility>
#include "codeGenerator.h"

using namespace std;

CodeSnippet::CodeSnippet(std::string command) {
    this->command = command;
    label = "";
    firstValue = "";
    secondValue = "";
}

CodeSnippet::CodeSnippet(int label, std::string command) {
    this->command = command;
    this->label = label;
    firstValue = "";
    secondValue = "";
}

CodeSnippet::CodeSnippet(std::string command, int firstValue) {
    this->command = command;
    label = "";
    this->firstValue = firstValue;
    secondValue = "";
}

CodeSnippet::CodeSnippet(int label, std::string command, int firstValue) {
    this->command = command;
    this->label = label;
    this->firstValue = firstValue;
    secondValue = "";
}

CodeSnippet::CodeSnippet(std::string command, int firstValue, int secondValue) {
    this->command = command;
    label = "";
    this->firstValue = firstValue;
    this->secondValue = secondValue;
}

CodeSnippet::CodeSnippet(int label, std::string command, int firstValue, int secondValue) {
    this->command = command;
    this->label = label;
    this->firstValue = firstValue;
    this->secondValue = secondValue;
}

CodeGenerator::CodeGenerator() { head = nullptr; }

void CodeGenerator::insertNode(CodeSnippet *code) {

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
