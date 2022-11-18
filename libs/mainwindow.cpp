#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "utils.h"
#include "lexicalAnalyser.h"
#include "syntacticAnalyser.h"
#include "semanticAnalyser.h"
#include "codeGenerator.h"
#include <unistd.h>

// ToDo descobrir um modo de quando algum erro que utiliza exit(1) não crashar o QT

using namespace std;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    this->setWindowTitle("Compilador");

    ui->compilarButton->setEnabled(false);
    ui->errorArea->setEnabled(false);
}

void MainWindow::on_codeArea_textChanged() {
    if (ui->codeArea->document()->isEmpty()) {
        ui->compilarButton->setEnabled(false);
    } else {
        ui->compilarButton->setEnabled(true);
    }
}

void MainWindow::on_compilarButton_clicked() {
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

    ui->errorArea->clear();

    sleep(1);

    Node token;
    lineNo = 1;

    FILE *f = openFile((char *) currentFile.toStdString().c_str());

    character = (char) fgetc(f);

    CodeGenerator codeGen;
    auto *snippet = new CodeSnippet(10, "SUM", 2, 3);
    codeGen.insertNode(snippet);
    codeGen.printList();
    codeGen.generateCode();

    do {
        token = getToken(f);

        if (!token.lexema.empty() && !token.simbolo.empty()) {
            if (token.simbolo == "sprograma") {
                token = getToken(f);

                if (token.simbolo == "sidentificador") {
                    symbolTable.downLayer(token.lexema, token.lexema, token.lexema, "programa", lineNo);

                    token = getToken(f);

                    if (token.simbolo == "sponto_virgula") {
                        token = analyseBlock(f, token, this->ui);

                        if (token.simbolo == "sponto") {
                            ui->errorArea->appendPlainText("Sucesso");
                        } else {
                            cout << "Erro25" << endl;
                        }
                    } else {
                        cout << "Erro26" << endl;
                    }
                } else {
                    cout << "Erro27" << endl;
                }
            } else {
                if (character != EOF) {
                    cout << "Erro28" << endl;
                }
            }
        } else {
            cout << "Erro29" << endl;
        }

    } while (character != EOF);

    fclose(f);
    cout << lineNo << endl;
}

void MainWindow::on_openFileButton_clicked() {
    ui->codeArea->clear();
    QString filename = QFileDialog::getOpenFileName(this, "Open File",
                                                    "C://Users//renat//CLionProjects//Compiler//testes",
                                                    "Text File (*.txt);");
    QFile file(filename);
    currentFile = filename;

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file!");
    }

    QTextStream stream(&file);
    QString text = stream.readAll();
    ui->codeArea->appendPlainText(text);
    ui->codeArea->verticalScrollBar()->setValue(0);
    file.close();
}

void MainWindow::on_actionOpen_triggered() {
    ui->codeArea->clear();
    QString filename = QFileDialog::getOpenFileName(this, "Open File",
                                                    "C://Users//renat//CLionProjects//Compiler//testes",
                                                    "Text File (*.txt);");
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot open file!");
        return;
    }

    currentFile = filename;

    QTextStream stream(&file);
    QString text = stream.readAll();
    ui->codeArea->appendPlainText(text);
    ui->codeArea->verticalScrollBar()->setValue(0);
    file.close();
}

void MainWindow::on_actionSave_triggered() {
    if (currentFile == "") {
        on_actionSave_as_triggered();
    } else {
        QSaveFile file(currentFile);
        file.open(QIODevice::WriteOnly);

        QByteArray outputByteArray;
        outputByteArray.append(ui->codeArea->toPlainText().toUtf8());

        file.write(outputByteArray);
        file.commit();
    }
}


void MainWindow::on_actionSave_as_triggered() {
    QString filename = QFileDialog::getSaveFileName(this, "Save as",
                                                    "C://Users//renat//CLionProjects//Compiler//testes",
                                                    "Text File (*.txt);");
    QFile file(filename);

    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "Warning", "Cannot save file!");
        return;
    }

    currentFile = filename;
    QTextStream stream(&file);
    QString text = ui->codeArea->toPlainText();
    stream << text;
    file.close();

    ui->errorArea->setPlainText(currentFile);
}

void MainWindow::on_actionSobre_o_Compilador_triggered() {
    QMessageBox::information(this, "Information",
                             "Compilador desenvolvido pelos alunos da PUC-Campinas \n Henrique Victorino Dias - 19030550\n Renato Barba dos Santos - 19246529 \n Vinicius Rabelo Mancini - 19006568");
}

MainWindow::~MainWindow() {
    delete ui;
}


