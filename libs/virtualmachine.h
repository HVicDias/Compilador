#ifndef COMPILER_VIRTUALMACHINE_H
#define COMPILER_VIRTUALMACHINE_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QPlainTextEdit>
#include <QScrollBar>
#include <QMessageBox>
#include <QSaveFile>
#include <windows.h>
#include <unistd.h>
#include <QThread>
#include <iostream>
#include <QInputDialog>
#include <QTableWidgetItem>
#include "semanticAnalyser.h"
#include <QFont>
#include <cstring>

namespace Ui {
    class VirtualMachine;
}

class VirtualMachine : public QMainWindow {
Q_OBJECT

public:
    explicit VirtualMachine(QWidget *parent = nullptr);

    ~VirtualMachine() override;

    void formatCodigoDaMaquinaLine(std::string);

    bool operationalAnalyser();

    bool loaderAnalyser();

    bool inverterAnalyser();

    bool storageAnalyser();

    bool pcChangerAnalyser();

    bool labelAnalyser();

    bool allocDallocAnalyser();

    bool uiInteractionsAnalyser();

    bool startHLTAnalyser();

    bool executeLine();

private slots:

    void on_NormalRadioButton_clicked();

    void on_PassoAPassoRadioButton_clicked();

    void addRowCodigoDaMaquinaTable(QString, QString, QString, QString);

    void addRowMemoriaTable(QString, QString);

    void removeRowMemoriaTable(int row);

    void on_actionOpen_triggered();

    void on_ExecutarButton_clicked();

private:
    Ui::VirtualMachine *ui;
    QString currentFile = "";
    QString mode = "normal";
    int programCounter = 0;
};

#endif //COMPILER_VIRTUALMACHINE_H
