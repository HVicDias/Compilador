#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../../../libs/utils.h"
#include "../../../libs/lexicalAnalyser.h"
#include "../../../libs/syntacticAnalyser.h"
#include "../../../libs/semanticAnalyser.h"
#include "../../../libs/codeGenerator.h"

using namespace std;

string filename;

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->setWindowTitle("Compilador");
    ui->setupUi(this);
    ui->compilarButton->setEnabled(false);

}

void MainWindow::on_openFileButton_clicked() {
    while (symbolTable.symbolListNode != nullptr) {
        symbolTable.deleteLayer();
    }

    filename = "";

    ui->codeArea->clear();

    QFile f(
            QFileDialog::getOpenFileName(
                    this,
                    tr("Open File"),
                    "C://Users//renat//CLionProjects//Compiler//testes",
                    "Text File (*.txt);"));

    f.open(QIODevice::ReadOnly | QIODevice::Text);

    filename = f.fileName().toStdString();

    QTextStream stream(&f);

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        ui->codeArea->appendPlainText(line);
    }
    ui->codeArea->verticalScrollBar()->setValue(0);

    f.close();
}

void MainWindow::on_codeArea_textChanged() {
    if (ui->codeArea->document()->isEmpty()) {
        ui->compilarButton->setEnabled(false);
    } else {
        ui->compilarButton->setEnabled(true);
    }
}

void MainWindow::on_compilarButton_clicked() {
    while (symbolTable.symbolListNode != nullptr) {
        symbolTable.deleteLayer();
    }

    ui->plainTextEdit->clear();

    Node token;
    lineNo = 1;

    FILE *f = openFile((char *) filename.c_str());

    character = (char) fgetc(f);

    CodeGenerator codeGen;
    auto *snippet = new CodeSnippet(10, "SUM", 2, 3);
    codeGen.insertNode(snippet);
    codeGen.printList();
    codeGen.generateCode();

//    ui->plainTextEdit->appendPlainText(QString::number(lineNo));
    do {
        token = getToken(f);

        if (!token.lexema.empty() && !token.simbolo.empty()) {
            if (token.simbolo == "sprograma") {
                token = getToken(f);

                if (token.simbolo == "sidentificador") {
                    symbolTable.downLayer(token.lexema, token.lexema, token.lexema, "programa", lineNo);

                    token = getToken(f);

                    if (token.simbolo == "sponto_virgula") {
                        token = analyseBlock(f, token);

                        if (token.simbolo == "sponto") {
                            ui->plainTextEdit->appendPlainText("Sucesso");
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

MainWindow::~MainWindow() {
    delete ui;
}


