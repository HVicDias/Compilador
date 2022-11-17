#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::MainWindow) {
    this->setWindowTitle("Compilador");
    ui->setupUi(this);
    ui->compilarButton->setEnabled(false);
}

void MainWindow::on_openFileButton_clicked() {
    ui->codeArea->clear();

    QFile file(
            QFileDialog::getOpenFileName(
                    this,
                    tr("Open File"),
                    "C://Users//renat//CLionProjects//Compiler//testes",
                    "Text File (*.txt);"));

    file.open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream stream(&file);

    while (!stream.atEnd()) {
        QString line = stream.readLine();
        ui->codeArea->appendPlainText(line);
    }
    ui->codeArea->verticalScrollBar()->setValue(0);
    file.close();
}

void MainWindow::on_codeArea_textChanged() {
    if (ui->codeArea->document()->isEmpty()) {
        ui->compilarButton->setEnabled(false);
    } else {
        ui->compilarButton->setEnabled(true);
    }
}

MainWindow::~MainWindow() {
    delete ui;
}


