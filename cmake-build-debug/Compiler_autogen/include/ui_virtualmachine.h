#ifndef UI_VIRTUALMACHINE_H
#define UI_VIRTUALMACHINE_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_VirtualMachine
{
public:
    QAction *actionOpen;
    QAction *actionSobreAMaquinaVirtual;
    QWidget *centralwidget;
    QLabel *CodigoDaMaquinaLabel;
    QLabel *MemoriaLabel;
    QLabel *SaidaDeDadosLabel;
    QLabel *ModoDeExecucaoLabel;
    QRadioButton *NormalRadioButton;
    QRadioButton *PassoAPassoRadioButton;
    QPlainTextEdit *SaidaDeDadosArea;
    QTableWidget *CodigoDaMaquinaTable;
    QTableWidget *MemoriaTable;
    QPushButton *ExecutarButton;
    QLabel *ArquivoName;
    QLabel *ArquivoLabel;
    QMenuBar *menubar;
    QMenu *menuArquivo;
    QMenu *menuSobre;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *VirtualMachine)
    {
        if (VirtualMachine->objectName().isEmpty())
            VirtualMachine->setObjectName("VirtualMachine");
        VirtualMachine->resize(1280, 720);
        actionOpen = new QAction(VirtualMachine);
        actionOpen->setObjectName("actionOpen");
        actionSobreAMaquinaVirtual = new QAction(VirtualMachine);
        actionSobreAMaquinaVirtual->setObjectName("actionSobreAMaquinaVirtual");
        centralwidget = new QWidget(VirtualMachine);
        centralwidget->setObjectName("centralwidget");
        CodigoDaMaquinaLabel = new QLabel(centralwidget);
        CodigoDaMaquinaLabel->setObjectName("CodigoDaMaquinaLabel");
        CodigoDaMaquinaLabel->setGeometry(QRect(20, 60, 201, 31));
        QFont font;
        font.setPointSize(13);
        CodigoDaMaquinaLabel->setFont(font);
        MemoriaLabel = new QLabel(centralwidget);
        MemoriaLabel->setObjectName("MemoriaLabel");
        MemoriaLabel->setGeometry(QRect(1010, 60, 161, 31));
        MemoriaLabel->setFont(font);
        SaidaDeDadosLabel = new QLabel(centralwidget);
        SaidaDeDadosLabel->setObjectName("SaidaDeDadosLabel");
        SaidaDeDadosLabel->setGeometry(QRect(20, 450, 151, 31));
        SaidaDeDadosLabel->setFont(font);
        ModoDeExecucaoLabel = new QLabel(centralwidget);
        ModoDeExecucaoLabel->setObjectName("ModoDeExecucaoLabel");
        ModoDeExecucaoLabel->setGeometry(QRect(670, 160, 191, 31));
        ModoDeExecucaoLabel->setFont(font);
        NormalRadioButton = new QRadioButton(centralwidget);
        NormalRadioButton->setObjectName("NormalRadioButton");
        NormalRadioButton->setGeometry(QRect(690, 190, 112, 31));
        NormalRadioButton->setFont(font);
        PassoAPassoRadioButton = new QRadioButton(centralwidget);
        PassoAPassoRadioButton->setObjectName("PassoAPassoRadioButton");
        PassoAPassoRadioButton->setGeometry(QRect(690, 220, 161, 31));
        PassoAPassoRadioButton->setFont(font);
        SaidaDeDadosArea = new QPlainTextEdit(centralwidget);
        SaidaDeDadosArea->setObjectName("SaidaDeDadosArea");
        SaidaDeDadosArea->setGeometry(QRect(20, 490, 521, 171));
        CodigoDaMaquinaTable = new QTableWidget(centralwidget);
        if (CodigoDaMaquinaTable->columnCount() < 4)
            CodigoDaMaquinaTable->setColumnCount(4);
        QTableWidgetItem *__qtablewidgetitem = new QTableWidgetItem();
        CodigoDaMaquinaTable->setHorizontalHeaderItem(0, __qtablewidgetitem);
        QTableWidgetItem *__qtablewidgetitem1 = new QTableWidgetItem();
        CodigoDaMaquinaTable->setHorizontalHeaderItem(1, __qtablewidgetitem1);
        QTableWidgetItem *__qtablewidgetitem2 = new QTableWidgetItem();
        CodigoDaMaquinaTable->setHorizontalHeaderItem(2, __qtablewidgetitem2);
        QTableWidgetItem *__qtablewidgetitem3 = new QTableWidgetItem();
        CodigoDaMaquinaTable->setHorizontalHeaderItem(3, __qtablewidgetitem3);
        CodigoDaMaquinaTable->setObjectName("CodigoDaMaquinaTable");
        CodigoDaMaquinaTable->setGeometry(QRect(20, 100, 501, 341));
        MemoriaTable = new QTableWidget(centralwidget);
        if (MemoriaTable->columnCount() < 2)
            MemoriaTable->setColumnCount(2);
        QTableWidgetItem *__qtablewidgetitem4 = new QTableWidgetItem();
        MemoriaTable->setHorizontalHeaderItem(0, __qtablewidgetitem4);
        QTableWidgetItem *__qtablewidgetitem5 = new QTableWidgetItem();
        MemoriaTable->setHorizontalHeaderItem(1, __qtablewidgetitem5);
        MemoriaTable->setObjectName("MemoriaTable");
        MemoriaTable->setGeometry(QRect(1010, 100, 251, 341));
        ExecutarButton = new QPushButton(centralwidget);
        ExecutarButton->setObjectName("ExecutarButton");
        ExecutarButton->setGeometry(QRect(680, 290, 171, 91));
        QFont font1;
        font1.setPointSize(13);
        font1.setBold(true);
        ExecutarButton->setFont(font1);
        ExecutarButton->setAutoFillBackground(false);
        ArquivoName = new QLabel(centralwidget);
        ArquivoName->setObjectName("ArquivoName");
        ArquivoName->setGeometry(QRect(90, 20, 681, 20));
        QFont font2;
        font2.setPointSize(11);
        ArquivoName->setFont(font2);
        ArquivoLabel = new QLabel(centralwidget);
        ArquivoLabel->setObjectName("ArquivoLabel");
        ArquivoLabel->setGeometry(QRect(20, 20, 81, 20));
        ArquivoLabel->setFont(font);
        VirtualMachine->setCentralWidget(centralwidget);
        menubar = new QMenuBar(VirtualMachine);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 1280, 25));
        menuArquivo = new QMenu(menubar);
        menuArquivo->setObjectName("menuArquivo");
        menuSobre = new QMenu(menubar);
        menuSobre->setObjectName("menuSobre");
        VirtualMachine->setMenuBar(menubar);
        statusbar = new QStatusBar(VirtualMachine);
        statusbar->setObjectName("statusbar");
        VirtualMachine->setStatusBar(statusbar);

        menubar->addAction(menuArquivo->menuAction());
        menubar->addAction(menuSobre->menuAction());
        menuArquivo->addAction(actionOpen);
        menuSobre->addAction(actionSobreAMaquinaVirtual);

        retranslateUi(VirtualMachine);

        QMetaObject::connectSlotsByName(VirtualMachine);
    } // setupUi

    void retranslateUi(QMainWindow *VirtualMachine)
    {
        VirtualMachine->setWindowTitle(QCoreApplication::translate("VirtualMachine", "MainWindow", nullptr));
        actionOpen->setText(QCoreApplication::translate("VirtualMachine", "Abrir", nullptr));
        actionSobreAMaquinaVirtual->setText(QCoreApplication::translate("VirtualMachine", "Sobre a M\303\241quina Virtual", nullptr));
        CodigoDaMaquinaLabel->setText(QCoreApplication::translate("VirtualMachine", "C\303\263digo da M\303\241quina", nullptr));
        MemoriaLabel->setText(QCoreApplication::translate("VirtualMachine", "Mem\303\263ria (Pilha)", nullptr));
        SaidaDeDadosLabel->setText(QCoreApplication::translate("VirtualMachine", "Sa\303\255da de Dados", nullptr));
        ModoDeExecucaoLabel->setText(QCoreApplication::translate("VirtualMachine", "Modo de Execu\303\247\303\243o", nullptr));
        NormalRadioButton->setText(QCoreApplication::translate("VirtualMachine", "Normal", nullptr));
        PassoAPassoRadioButton->setText(QCoreApplication::translate("VirtualMachine", "Passo a Passo", nullptr));
        QTableWidgetItem *___qtablewidgetitem = CodigoDaMaquinaTable->horizontalHeaderItem(0);
        ___qtablewidgetitem->setText(QCoreApplication::translate("VirtualMachine", "Linha", nullptr));
        QTableWidgetItem *___qtablewidgetitem1 = CodigoDaMaquinaTable->horizontalHeaderItem(1);
        ___qtablewidgetitem1->setText(QCoreApplication::translate("VirtualMachine", "Instru\303\247\303\243o", nullptr));
        QTableWidgetItem *___qtablewidgetitem2 = CodigoDaMaquinaTable->horizontalHeaderItem(2);
        ___qtablewidgetitem2->setText(QCoreApplication::translate("VirtualMachine", "Atributo 1", nullptr));
        QTableWidgetItem *___qtablewidgetitem3 = CodigoDaMaquinaTable->horizontalHeaderItem(3);
        ___qtablewidgetitem3->setText(QCoreApplication::translate("VirtualMachine", "Atributo 2", nullptr));
        QTableWidgetItem *___qtablewidgetitem4 = MemoriaTable->horizontalHeaderItem(0);
        ___qtablewidgetitem4->setText(QCoreApplication::translate("VirtualMachine", "Endere\303\247o", nullptr));
        QTableWidgetItem *___qtablewidgetitem5 = MemoriaTable->horizontalHeaderItem(1);
        ___qtablewidgetitem5->setText(QCoreApplication::translate("VirtualMachine", "Valor", nullptr));
        ExecutarButton->setText(QCoreApplication::translate("VirtualMachine", "Executar", nullptr));
        ArquivoName->setText(QString());
        ArquivoLabel->setText(QCoreApplication::translate("VirtualMachine", "Arquivo:", nullptr));
        menuArquivo->setTitle(QCoreApplication::translate("VirtualMachine", "Arquivo", nullptr));
        menuSobre->setTitle(QCoreApplication::translate("VirtualMachine", "Sobre", nullptr));
    } // retranslateUi

};

namespace Ui {
    class VirtualMachine: public Ui_VirtualMachine {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_VIRTUALMACHINE_H
