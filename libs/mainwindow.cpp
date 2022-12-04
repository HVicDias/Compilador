#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "lexicalAnalyser.h"
#include "syntacticAnalyser.h"
#include "semanticAnalyser.h"
#include <unistd.h>

using namespace std;
CodeGenerator codeGen;
string mainProgramIndentifier;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    this->setWindowTitle("Compilador");

    ui->CompilarButton->setEnabled(false);
    ui->ErrorArea->setReadOnly(true);
}

void MainWindow::on_CodeArea_textChanged() {
    if (ui->CodeArea->document()->isEmpty()) {
        ui->CompilarButton->setEnabled(false);
    } else {
        ui->CompilarButton->setEnabled(true);
    }
}

void MainWindow::on_CompilarButton_clicked() {
    if (currentFile == "") {
        while (currentFile == "") {
            on_actionSave_as_triggered();
        }
    } else {
        on_actionSave_triggered();
    }

    while (symbolTable.symbolListNode != nullptr) {
        symbolTable.deleteLayer();
    }
    currentMemoryAllocation = 1;
    currentLabel = 0;
    lastReturn = false;
    hadPop = false;
    ui->ErrorArea->clear();
    ui->ErrorArea->setPlainText("");

    sleep(1);

    Node token;
    lineNo = 1;

    FILE *f = openFile((char *) currentFile.toStdString().c_str());

    character = (char) fgetc(f);
    auto *snippet = new CodeSnippet("START");

    token = getToken(f, ui);

    if (!token.lexema.empty() && !token.simbolo.empty()) {
        if (token.simbolo == "sprograma") {
            token = getToken(f, ui);

            if (token.simbolo == "sidentificador") {
                mainProgramIndentifier = token.lexema;
                symbolTable.downLayer(token.lexema, token.lexema, token.lexema, "programa",
                                      lineNo == 1 ? lineNo : lineNo + 1, -1, -1, -1);
                headerStack.push(symbolTable.insertSymbol(token.lexema, symbolTable.symbolListNode->layerName,
                                                          "programa", lineNo == 1 ? lineNo : lineNo + 1, -1, -1,
                                                          -1));

                token = getToken(f, ui);

                if (token.simbolo == "sponto_virgula") {
                    codeGen.insertNode(snippet);
                    snippet = new CodeSnippet("ALLOC", 0, 1);
                    codeGen.insertNode(snippet);

                    token = analyseBlock(f, token, this->ui);

                    if (token.simbolo == "sponto") {
                        if (ui->ErrorArea->toPlainText().isEmpty()) {
                            vm = new VirtualMachine(this);
                            vm->show();
                        }
                    } else {
                        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo + 1) +
                                                        ": Erro Sintático -> Esperado \".\"."));
                    }
                } else {
                    ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo + 1) +
                                                    ": Erro Sintático -> Esperado \";\"."));
                }
            } else {
                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo + 1) +
                                                ": Erro Sintático -> Esperado um identificador."));
            }
        } else {
            if (character != EOF) {
                ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo + 1) +
                                                ": Erro Sintático -> Esperado \"programa\"."));
            }
        }
    } else {
        ui->ErrorArea->appendPlainText(("Linha " + QString::number(lineNo) +
                                        ": Erro Léxico -> Caracter inválido."));
    }

    int numberDeletion = symbolTable.deleteLayer();
    if (numberDeletion != 0) {
        snippet = new CodeSnippet("DALLOC", currentMemoryAllocation - numberDeletion, numberDeletion);
        currentMemoryAllocation -= numberDeletion;
        codeGen.insertNode(snippet);
    }
    snippet = new CodeSnippet("DALLOC", currentMemoryAllocation - 1, 1);
    codeGen.insertNode(snippet);
    snippet = new CodeSnippet("HLT");
    codeGen.insertNode(snippet);


    if (ui->ErrorArea->toPlainText().isEmpty()) {
        codeGen.printList();
        codeGen.generateCode();
        codeGen.deleteCode();
    }

    fclose(f);
    cout << lineNo << endl;
}

void MainWindow::on_actionOpen_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "Abrir Arquivo",
                                                    "C://Users//renat//CLionProjects//Compiler//testes",
                                                    "Text File (*.txt);");
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    ui->CodeArea->clear();
    currentFile = filename;

    QTextStream stream(&file);
    QString text = stream.readAll();
    ui->ArquivoName->setText(filename);
    ui->CodeArea->appendPlainText(text);
    ui->CodeArea->verticalScrollBar()->setValue(0);
    ui->ErrorArea->clear();

    file.close();
}

void MainWindow::on_actionSave_triggered() {
    if (currentFile == "") {
        on_actionSave_as_triggered();
    } else {
        QSaveFile file(currentFile);
        file.open(QIODevice::WriteOnly);

        QByteArray outputByteArray;
        outputByteArray.append(ui->CodeArea->toPlainText().toUtf8());

        ui->ErrorArea->clear();
        file.write(outputByteArray);
        file.commit();
    }
}

void MainWindow::on_actionSave_as_triggered() {
    QString filename = QFileDialog::getSaveFileName(this, "Salvar como",
                                                    "C://Users//renat//CLionProjects//Compiler//testes",
                                                    "Text File (*.txt);");
    QFile file(filename);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        return;
    }

    currentFile = filename;
    ui->ArquivoName->setText(filename);
    QTextStream stream(&file);
    QString text = ui->CodeArea->toPlainText();
    stream << text;
    file.close();

    ui->ErrorArea->clear();
}

void MainWindow::on_actionAbrir_Maquina_Virtual_triggered() {
    vm = new VirtualMachine(this);
    vm->show();
}

void MainWindow::on_actionSobre_o_Compilador_triggered() {
    QMessageBox::information(this, "Information",
                             "Compilador desenvolvido pelos alunos da PUC-Campinas \n Henrique Victorino Dias - 19030550\n Renato Barba dos Santos - 19246529 \n Vinicius Rabelo Mancini - 19006568");
}

MainWindow::~MainWindow() {
    delete ui;
}


