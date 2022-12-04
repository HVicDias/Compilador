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
#include "virtualmachine.h"
#include "codeGenerator.h"
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <QFuture>

extern CodeGenerator codeGen;
extern std::string mainProgramIndentifier;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow() override;

private slots:

    void on_CodeArea_textChanged();

    void on_CompilarButton_clicked();

    void on_actionOpen_triggered();

    void on_actionSave_as_triggered();

    void on_actionSave_triggered();

    void on_actionSobre_o_Compilador_triggered();

    void on_actionAbrir_Maquina_Virtual_triggered();

private:
    Ui::MainWindow *ui;
    QString currentFile = "";

    VirtualMachine *vm;
};

#endif //COMPILER_MAINWINDOW_H
