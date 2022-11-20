#ifndef COMPILER_MAINWINDOW_H
#define COMPILER_MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QMessageBox>
#include <QSaveFile>
#include "codeGenerator.h"

extern CodeGenerator codeGen;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

    void on_openFileButton_clicked();

    void on_codeArea_textChanged();

    void on_compilarButton_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionSave_triggered();

    void on_actionSobre_o_Compilador_triggered();


private:
    Ui::MainWindow *ui;
    QString currentFile = "";
};

#endif //COMPILER_MAINWINDOW_H
