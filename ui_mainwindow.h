/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.4.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>
#include "../../../gui/CodeEditor/codeeditor.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionSave;
    QAction *actionSave_as;
    QAction *actionOpen;
    QAction *actionSobre_o_Compilador;
    QAction *actionAbrir_Maquina_Virtual;
    QWidget *centralwidget;
    QLabel *ArquivoLabel;
    CodeEditor *CodeArea;
    QLabel *ErrorLabel;
    QPlainTextEdit *ErrorArea;
    QPushButton *CompilarButton;
    QLabel *ArquivoName;
    QMenuBar *menubar;
    QMenu *menuArquivo;
    QMenu *menuSobre;
    QMenu *menuMaquina_Virtual;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionSave = new QAction(MainWindow);
        actionSave->setObjectName("actionSave");
        actionSave_as = new QAction(MainWindow);
        actionSave_as->setObjectName("actionSave_as");
        actionOpen = new QAction(MainWindow);
        actionOpen->setObjectName("actionOpen");
        actionSobre_o_Compilador = new QAction(MainWindow);
        actionSobre_o_Compilador->setObjectName("actionSobre_o_Compilador");
        actionAbrir_Maquina_Virtual = new QAction(MainWindow);
        actionAbrir_Maquina_Virtual->setObjectName("actionAbrir_Maquina_Virtual");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        ArquivoLabel = new QLabel(centralwidget);
        ArquivoLabel->setObjectName("ArquivoLabel");
        ArquivoLabel->setGeometry(QRect(20, 30, 81, 20));
        QFont font;
        font.setPointSize(13);
        ArquivoLabel->setFont(font);
        CodeArea = new CodeEditor(centralwidget);
        CodeArea->setObjectName("CodeArea");
        CodeArea->setGeometry(QRect(20, 80, 751, 321));
        ErrorLabel = new QLabel(centralwidget);
        ErrorLabel->setObjectName("ErrorLabel");
        ErrorLabel->setGeometry(QRect(20, 430, 63, 20));
        ErrorLabel->setFont(font);
        ErrorArea = new QPlainTextEdit(centralwidget);
        ErrorArea->setObjectName("ErrorArea");
        ErrorArea->setGeometry(QRect(20, 460, 501, 101));
        ErrorArea->setStyleSheet(QString::fromUtf8("background-color: white"));
        CompilarButton = new QPushButton(centralwidget);
        CompilarButton->setObjectName("CompilarButton");
        CompilarButton->setGeometry(QRect(610, 470, 131, 61));
        CompilarButton->setFont(font);
        ArquivoName = new QLabel(centralwidget);
        ArquivoName->setObjectName("ArquivoName");
        ArquivoName->setGeometry(QRect(90, 30, 681, 20));
        QFont font1;
        font1.setPointSize(11);
        ArquivoName->setFont(font1);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 25));
        menuArquivo = new QMenu(menubar);
        menuArquivo->setObjectName("menuArquivo");
        menuSobre = new QMenu(menubar);
        menuSobre->setObjectName("menuSobre");
        menuMaquina_Virtual = new QMenu(menubar);
        menuMaquina_Virtual->setObjectName("menuMaquina_Virtual");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuArquivo->menuAction());
        menubar->addAction(menuMaquina_Virtual->menuAction());
        menubar->addAction(menuSobre->menuAction());
        menuArquivo->addAction(actionOpen);
        menuArquivo->addAction(actionSave);
        menuArquivo->addAction(actionSave_as);
        menuSobre->addAction(actionSobre_o_Compilador);
        menuMaquina_Virtual->addAction(actionAbrir_Maquina_Virtual);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionSave->setText(QCoreApplication::translate("MainWindow", "Salvar", nullptr));
        actionSave_as->setText(QCoreApplication::translate("MainWindow", "Salvar Como", nullptr));
        actionOpen->setText(QCoreApplication::translate("MainWindow", "Abrir", nullptr));
        actionSobre_o_Compilador->setText(QCoreApplication::translate("MainWindow", "Sobre o Compilador", nullptr));
        actionAbrir_Maquina_Virtual->setText(QCoreApplication::translate("MainWindow", "Abrir M\303\241quina Virtual", nullptr));
        ArquivoLabel->setText(QCoreApplication::translate("MainWindow", "Arquivo:", nullptr));
        ErrorLabel->setText(QCoreApplication::translate("MainWindow", "Erros", nullptr));
        CompilarButton->setText(QCoreApplication::translate("MainWindow", "Compilar", nullptr));
        ArquivoName->setText(QString());
        menuArquivo->setTitle(QCoreApplication::translate("MainWindow", "Arquivo", nullptr));
        menuSobre->setTitle(QCoreApplication::translate("MainWindow", "Sobre", nullptr));
        menuMaquina_Virtual->setTitle(QCoreApplication::translate("MainWindow", "M\303\241quina Virtual", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
