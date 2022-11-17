#ifndef COMPILER_MAINWINDOW_H
#define COMPILER_MAINWINDOW_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QScrollBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:

    void on_openFileButton_clicked();

    void on_codeArea_textChanged();


private:
    Ui::MainWindow *ui;
};

#endif //COMPILER_MAINWINDOW_H
