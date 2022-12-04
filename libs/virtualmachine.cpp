#include "virtualmachine.h"
#include "ui_virtualmachine.h"
#include <stack>
#include <string>
#include <list>
#include <tuple>

std::stack<int> returnStack; // PC return values
std::list<std::tuple<int, int, int>> memoryStack; // {memoryAlocation, value, memoryReference}
std::list<std::tuple<int, std::string>> labelJmp; // {PC, label}
int compilerMemoryAllocation = -1;

VirtualMachine::VirtualMachine(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::VirtualMachine) {

    ui->setupUi(this);
    this->setWindowTitle("Máquina Virtual");
    ui->NormalRadioButton->setChecked(true);
    ui->CodigoDaMaquinaTable->verticalHeader()->setVisible(false);
    ui->MemoriaTable->verticalHeader()->setVisible(false);
    ui->CodigoDaMaquinaTable->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->CodigoDaMaquinaTable->setDisabled(true);
    ui->MemoriaTable->setDisabled(true);
    ui->MemoriaTable->setSelectionMode(QAbstractItemView::NoSelection);
    ui->MemoriaTable->setFocusPolicy(Qt::NoFocus);
    ui->SaidaDeDadosArea->setReadOnly(true);
    ui->ExecutarButton->setDisabled(true);
}

void VirtualMachine::addRowCodigoDaMaquinaTable(QString line, QString command, QString attr1, QString attr2) {
    ui->CodigoDaMaquinaTable->insertRow(ui->CodigoDaMaquinaTable->rowCount());
    ui->CodigoDaMaquinaTable->setItem(ui->CodigoDaMaquinaTable->rowCount() - 1, 0, new QTableWidgetItem(line));
    ui->CodigoDaMaquinaTable->setItem(ui->CodigoDaMaquinaTable->rowCount() - 1, 1, new QTableWidgetItem(command));
    ui->CodigoDaMaquinaTable->setItem(ui->CodigoDaMaquinaTable->rowCount() - 1, 2, new QTableWidgetItem(attr1));
    ui->CodigoDaMaquinaTable->setItem(ui->CodigoDaMaquinaTable->rowCount() - 1, 3, new QTableWidgetItem(attr2));
}

void VirtualMachine::addRowMemoriaTable(QString address, QString value) {
    ui->MemoriaTable->insertRow(0);
    ui->MemoriaTable->setItem(0, 0, new QTableWidgetItem(address));
    ui->MemoriaTable->setItem(0, 1, new QTableWidgetItem(value));
}

void VirtualMachine::removeRowMemoriaTable(int row = 0) {
    ui->MemoriaTable->removeRow(row);
}

VirtualMachine::~VirtualMachine() {
    delete ui;
}

void VirtualMachine::on_NormalRadioButton_clicked() {
    mode = "normal";
}


void VirtualMachine::on_PassoAPassoRadioButton_clicked() {
    mode = "passo";
}


void VirtualMachine::on_actionOpen_triggered() {
    QString filename = QFileDialog::getOpenFileName(this, "Abrir Arquivo",
                                                    "C://Users//renat//CLionProjects//Compiler//cmake-build-debug",
                                                    "Object File (*.obj);");
    QFile file(filename);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return;
    }

    currentFile = filename;
    
    ui->CodigoDaMaquinaTable->setRowCount(0);
    ui->MemoriaTable->setRowCount(0);

    QTextStream stream(&file);
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        formatCodigoDaMaquinaLine(line.toStdString());
    }
    programCounter = 0;
    ui->CodigoDaMaquinaTable->scrollToTop();
    ui->SaidaDeDadosArea->setPlainText("");

    compilerMemoryAllocation = -1;
    while (!returnStack.empty()) {
        returnStack.pop();
    }

    while (!memoryStack.empty()) {
        memoryStack.pop_back();
    }

    while (!labelJmp.empty()) {
        labelJmp.pop_back();
    }

    for (int i = 0; i < ui->CodigoDaMaquinaTable->rowCount() - 1; i++) {
        if (ui->CodigoDaMaquinaTable->item(i, 0)->text() != "")
            labelJmp.push_front(std::make_tuple(i,
                                                ui->CodigoDaMaquinaTable->item(i, 0)->text().toStdString()));
    }

    ui->ExecutarButton->setEnabled(true);

    file.close();

}

void VirtualMachine::formatCodigoDaMaquinaLine(std::string line) {
    std::string p1, p2, p3, p4;
    p1 = std::string(&line[0], &line[4]);
    p2 = std::string(&line[4], &line[12]);
    p3 = std::string(&line[12], &line[16]);
    p4 = std::string(&line[16], &line[20]);

    addRowCodigoDaMaquinaTable(QString::fromStdString(p1).trimmed(),
                               QString::fromStdString(p2).trimmed(),
                               QString::fromStdString(p3).trimmed(),
                               QString::fromStdString(p4).trimmed());
}

//Operacional
//Inverter
//Carregar
//Guardar
//Mudança de PC
//Colocar Label
//Alocar/Liberar espaço
//Interações de UI
//Simbolo de inicio/final


bool VirtualMachine::operationalAnalyser() {
    int value1, value2, result;

    if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "ADD") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        compilerMemoryAllocation--;
        result = value1 + value2;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        removeRowMemoriaTable();
        ui->MemoriaTable->item(0, 1)->setText(QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "SUB") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        compilerMemoryAllocation--;
        result = value2 - value1;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        removeRowMemoriaTable();
        ui->MemoriaTable->item(0, 1)->setText(QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "MULT") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        compilerMemoryAllocation--;
        result = value1 * value2;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        removeRowMemoriaTable();
        ui->MemoriaTable->item(0, 1)->setText(QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "DIVI") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        compilerMemoryAllocation--;
        result = value2 / value1;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        removeRowMemoriaTable();
        ui->MemoriaTable->item(0, 1)->setText(QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "AND") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        result = value1 * value2;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "OR") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        result = value1 + value2;
        if (result == 2)
            result = 1;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "CME") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        result = value2 < value1;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "CMA") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        result = value2 > value1;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "CEQ") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        result = value1 == value2;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "CDIF") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        result = value1 != value2;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "CMEQ") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        result = value2 <= value1;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "CMAQ") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        value2 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        result = value2 >= value1;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
        return true;
    }

    return false;
}

bool VirtualMachine::inverterAnalyser() {
    int value1, result;
    if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "INV") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        compilerMemoryAllocation--;
        result = value1 * -1;
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        ui->MemoriaTable->item(0, 1)->setText(QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "NEG") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        compilerMemoryAllocation--;
        if (value1 == 1) {
            result = 0;
        } else {
            result = 1;
        }
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        ui->MemoriaTable->item(0, 1)->setText(QString::number(result));
        return true;
    }

    return false;
}

bool VirtualMachine::loaderAnalyser() {
    int value1, result;
    if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "LDC") {
        result = (ui->CodigoDaMaquinaTable->item(programCounter, 2)->text().toInt());
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "LDV") {
        value1 = (ui->CodigoDaMaquinaTable->item(programCounter, 2)->text().toInt());
        auto l_back = memoryStack.end();
        while (get<0>(*l_back) != value1 && get<2>(*l_back) != value1) {
            l_back = std::prev(l_back);
        }
        result = get<1>(*l_back);
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
        return true;
    }

    return false;
}


bool VirtualMachine::storageAnalyser() {
    int value1, value2;
    if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "STR") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        value2 = (ui->CodigoDaMaquinaTable->item(programCounter, 2)->text().toInt());
        auto l_back = memoryStack.end();
        while (get<0>(*l_back) != value2 && get<2>(*l_back) != value2) {
            l_back = std::prev(l_back);
        }
        get<1>(*l_back) = value1;
        ui->MemoriaTable->item(ui->MemoriaTable->rowCount() - value2 - 1, 1)->setText(
                QString::number(get<1>(*l_back)));
        return true;
    }

    return false;
}

bool VirtualMachine::pcChangerAnalyser() {
    int value1, result;
    if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "JMP") {
        value1 = (ui->CodigoDaMaquinaTable->item(programCounter, 2)->text().toInt());
        auto l_front = labelJmp.begin();
        while (get<1>(*l_front) != (ui->CodigoDaMaquinaTable->item(programCounter, 2)->text()).toStdString()) {
            std::advance(l_front, 1);
        }
        programCounter = get<0>(*l_front);
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "JMPF") {
        result = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        if (result == 0) {
            value1 = (ui->CodigoDaMaquinaTable->item(programCounter, 2)->text().toInt());
            auto l_front = labelJmp.begin();
            while (get<1>(*l_front) != (ui->CodigoDaMaquinaTable->item(programCounter, 2)->text()).toStdString()) {
                std::advance(l_front, 1);
            }
            programCounter = get<0>(*l_front);
        } else {
            programCounter++;
        }
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "CALL") {
        returnStack.push(programCounter);
        value1 = (ui->CodigoDaMaquinaTable->item(programCounter, 2)->text().toInt());
        auto l_front = labelJmp.begin();
        while (get<1>(*l_front) != (ui->CodigoDaMaquinaTable->item(programCounter, 2)->text()).toStdString()) {
            std::advance(l_front, 1);
        }
        programCounter = get<0>(*l_front);
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "RETURN") {
        programCounter = returnStack.top() + 1;
        returnStack.pop();
        return true;
    }

    return false;
}

bool VirtualMachine::labelAnalyser() {
    if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "NULL") {
        return true;
    }

    return false;
}

// 0 -> linha
// 1 -> instrução
// 2 -> attr1
// 3 -> attr2
//std::stack<int> returnStack PC return values
//std::list<std::tuple<int, int>> memoryStack {memoryAlocation, value}
//std::list<std::tuple<int, std::string>> labelJmp {PC, label}

bool VirtualMachine::allocDallocAnalyser() {
    int value1, value2, currentStackTop, result;
    if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "ALLOC") {
        value1 = (ui->CodigoDaMaquinaTable->item(programCounter, 3)->text().toInt());
        currentStackTop = (ui->CodigoDaMaquinaTable->item(programCounter, 2)->text().toInt());
        if (compilerMemoryAllocation < currentStackTop) {
            for (int i = 0; i < value1; i++) {
                memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, 0, 0));
                addRowMemoriaTable(QString::number(compilerMemoryAllocation), "0");
            }
        } else {
            for (int i = 0; i < value1; i++) {
                value2 = i + currentStackTop;
                auto l_back = memoryStack.end();
                while (get<0>(*l_back) != value2 && get<2>(*l_back) != value2) {
                    l_back = std::prev(l_back);
                }
                result = get<1>(*l_back);
                memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, i));
                addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
            }
        }

        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "DALLOC") {
        value1 = (ui->CodigoDaMaquinaTable->item(programCounter, 3)->text().toInt());

        for (int i = 0; i < value1; i++) {
            removeRowMemoriaTable();
            memoryStack.pop_back();
            compilerMemoryAllocation--;
        }
        return true;
    }

    return false;
}

bool isNumber(QString s) {
    for (int i = 0; i < s.size(); i++) {
        if (!s[i].isDigit())
            return false;
    }

    return true;
}

bool VirtualMachine::uiInteractionsAnalyser() {
    int value1, result;
    QString text;
    bool ok, ok2;

    if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "RD") {

        do {
            do {
                text = QInputDialog::getText(this, "Leia",
                                             "Digite o valor do identificador", QLineEdit::Normal,
                                             "", &ok);
            } while (text.isEmpty());
        } while (!isNumber(text));

        if (text == "verdadeiro") {
            text = "1";
        } else if (text == "falso") {
            text = "0";
        }

        result = text.toInt();
        memoryStack.push_back(std::make_tuple(++compilerMemoryAllocation, result, 0));
        addRowMemoriaTable(QString::number(compilerMemoryAllocation), QString::number(result));
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "PRN") {
        value1 = get<1>(memoryStack.back());
        memoryStack.pop_back();
        removeRowMemoriaTable();
        compilerMemoryAllocation--;
        //print
        ui->SaidaDeDadosArea->appendPlainText(QString::number(value1));
        return true;
    }

    return false;
}

bool VirtualMachine::startHLTAnalyser() {
    if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "START") {
        return true;
    } else if (ui->CodigoDaMaquinaTable->item(programCounter, 1)->text() == "HLT") {
        return true;
    }

    return false;
}

bool VirtualMachine::executeLine() {
    if (operationalAnalyser()) {
        programCounter++;
        return true;
    } else if (loaderAnalyser()) {
        programCounter++;
        return true;
    } else if (inverterAnalyser()) {
        programCounter++;
        return true;
    } else if (storageAnalyser()) {
        programCounter++;
        return true;
    } else if (pcChangerAnalyser()) {
        return true;
    } else if (labelAnalyser()) {
        programCounter++;
        return true;
    } else if (allocDallocAnalyser()) {
        programCounter++;
        return true;
    } else if (uiInteractionsAnalyser()) {
        programCounter++;
        return true;
    } else if (startHLTAnalyser()) {
        programCounter++;
        return true;
    }

    return false;
}

void VirtualMachine::on_ExecutarButton_clicked() {
    if (programCounter == ui->CodigoDaMaquinaTable->rowCount()) {
        programCounter = 0;
        ui->SaidaDeDadosArea->setPlainText("");
        compilerMemoryAllocation = -1;
        while (!returnStack.empty()) {
            returnStack.pop();
        }

        while (!memoryStack.empty()) {
            memoryStack.pop_back();
        }
    }

    if (mode == "normal") {
        while (programCounter < ui->CodigoDaMaquinaTable->rowCount()) {
            ui->CodigoDaMaquinaTable->selectRow(programCounter);

            executeLine();

            setStyleSheet("QTableView::item:selected {background-color: #fffc9c; color: black}");
        }
    } else {
        ui->CodigoDaMaquinaTable->selectRow(programCounter);

        executeLine();

        setStyleSheet("QTableView::item:selected {background-color: #fffc9c; color: black}");
    }
}

void VirtualMachine::on_actionSobreAMaquinaVirtual_triggered() {
    QMessageBox::information(this, "Information",
                             "Máquina Virtual desenvolvida pelos alunos da PUC-Campinas \n Henrique Victorino Dias - 19030550\n Renato Barba dos Santos - 19246529 \n Vinicius Rabelo Mancini - 19006568");

}
