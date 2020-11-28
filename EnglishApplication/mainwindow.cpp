#include "mainwindow.h"
#include "mycentralwidget.h"
#include <QMenu>
#include <QMenuBar>
#include <QApplication>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->setStyleSheet("QMenuBar {border: 1px solid #000000; background-color: #022021; color: #e7f7fa}"
                        "QMenuBar::item:selected {background: #252525}"
                        "QMenu {selection-background-color: #250a1b; background-color: #252525; color: #e7f7fa}");

    QMenuBar* bar = this->menuBar();
    QMenu* modeMenu = new QMenu(trUtf8("&Режим"), this);
    QAction* learnPage = new QAction(trUtf8("Учить"),this);
    learnPage->setShortcut(trUtf8("CTRL+1"));
    QAction* excPage = new QAction(trUtf8("Практиковаться"),this);
    excPage->setShortcut(trUtf8("CTRL+2"));
    QAction* addPage = new QAction(trUtf8("Редактировать"),this);
    addPage->setShortcut(trUtf8("CTRL+3"));
    QAction* quitAct = new QAction(trUtf8("Выход"), this);
    quitAct->setShortcut(trUtf8("CTRL+Q"));
    modeMenu->addAction(learnPage);
    modeMenu->addAction(excPage);
    modeMenu->addAction(addPage);
    modeMenu->addSeparator();
    modeMenu->addAction(quitAct);
    connect(quitAct, SIGNAL(triggered()), qApp, SLOT(quit()));

    QMenu* learnMenu = new QMenu(trUtf8("&Учить"),this);
    learnMenu->setDisabled(true);
    QMenu* practicMenu = new QMenu(trUtf8("&Практика"),this);
    practicMenu->setDisabled(true);
    QMenu* addMenu = new QMenu(trUtf8("Ре&дактировать"),this);
    addMenu->setDisabled(true);
    bar->addMenu(modeMenu);
    bar->addMenu(learnMenu);
    bar->addMenu(practicMenu);
    bar->addMenu(addMenu);
    menuList<<learnMenu<<practicMenu<<addMenu;
    MyCentralWidget* wgt = new MyCentralWidget(menuList, this);
    connect(learnPage, SIGNAL(triggered()), wgt, SLOT(btnStartToggled()));
    connect(excPage, SIGNAL(triggered()), wgt, SLOT(btnPracticeToggled()));
    connect(addPage, SIGNAL(triggered()), wgt, SLOT(btnAddToggled()));
    this->setCentralWidget(wgt);
}

MainWindow::~MainWindow()
{
}

