#include "mycentralwidget.h"
#include "addwordswidget.h"
#include "learnwidget.h"
#include "exercisewidget.h"
#include "database.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTabWidget>
#include <QDebug>
#include <QLineEdit>
#include <QLabel>
#include <QTabBar>
#include <QPalette>
#include <QColor>
#include <QMainWindow>
#include <QListWidget>
#include <QMenu>

MyCentralWidget::MyCentralWidget(QList<QMenu*>& list, QWidget *parent) : QWidget(parent), menulist(list)
{ 

    db = new DataBase(this);
    db->createConnection();
    //this->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
    //this->setStyleSheet("background-color: #022021");
    QHBoxLayout* m_hbx = new QHBoxLayout(this);
    //m_hbx->setSizeConstraint(QLayout::SetMaximumSize);
    m_hbx->setContentsMargins(0,0,0,0);
    m_hbx->setSpacing(0);

    QWidget* btn_wgt = new QWidget(this);
    btn_wgt->setMaximumWidth(100);
    m_hbx->addWidget(btn_wgt);
    btn_wgt->setStyleSheet("QPushButton {background-color: #022021; color: #e7f7fa}");
    btn_wgt->setStyleSheet("background-color: #343839");

    QPushButton* btnStart = new QPushButton(trUtf8("Учить"),btn_wgt);
    btnStart->setFixedWidth(100);
    btnStart->setMaximumHeight(70);
    btnStart->setFlat(true);
    btnStart->setCheckable(true);
    btnStart->setFocusPolicy(Qt::NoFocus);
    wgtStart = new LearnWidget(db,this);
    wgtStart->setMenuActions(menulist[0]);
    //wgtStart->setMinimumSize(500,500);
    //hbxStart = new QHBoxLayout(wgtStart);
    //QLabel* lbl2 = new QLabel("lbl2");
    //hbxStart->addWidget(lbl2);
    wgtStart->hide();
    //hbxStart->addWidget(wgtStart);
    connect(btnStart, SIGNAL(clicked()),this,SLOT(btnStartToggled()));

    QPushButton* btnPractice = new QPushButton(trUtf8("Практика"), btn_wgt);
    btnPractice->setFixedWidth(100);
    btnPractice->setMaximumHeight(70);
    btnPractice->setFlat(true);
    btnPractice->setCheckable(true);
    btnPractice->setFocusPolicy(Qt::NoFocus);
    wgtExc = new ExerciseWidget(db, this);
    wgtExc->setMenuActions(menulist[1]);
    wgtExc->hide();
    connect(btnPractice, SIGNAL(clicked()), this, SLOT(btnPracticeToggled()));

    QPushButton* btnAdd = new QPushButton(trUtf8("Словарь"), btn_wgt);
    btnAdd->setFixedWidth(100);
    btnAdd->setMaximumHeight(70);
    btnAdd->setFlat(true);
    btnAdd->setCheckable(true);
    btnAdd->setFocusPolicy(Qt::NoFocus);
    wgtAdd = new AddWordsWidget(db, this);
    wgtAdd->setMenuActions(menulist[2]);
    wgtAdd->hide();
    connect(wgtAdd, SIGNAL(signalForLearnWidget()), wgtStart, SLOT(updateLearnWidget()));
    connect(wgtAdd, SIGNAL(signalEdit(int)), wgtStart, SLOT(updateRecord(int)));
    connect(wgtAdd, SIGNAL(signalDelete(int)), wgtStart, SLOT(deleteRecord(int)));

    connect(btnAdd, SIGNAL(clicked()),this,SLOT(btnAddToggled()));

    QLabel* lbl = new QLabel(this);
    QVBoxLayout* but_vbx = new QVBoxLayout(btn_wgt);
    but_vbx->setContentsMargins(0,0,0,0);
    but_vbx->setSpacing(0);
    but_vbx->addWidget(btnStart);
    but_vbx->addWidget(btnPractice);
    but_vbx->addWidget(btnAdd);
    but_vbx->addWidget(lbl);

    mainWgt = new QWidget(this);

    QLabel* mainlbl = new QLabel(mainWgt);
    QHBoxLayout* mainhbx = new QHBoxLayout(mainWgt);
    mainhbx->addWidget(mainlbl);


    lst<<(QWidget*)wgtStart<<(QWidget*)wgtExc<<(QWidget*)wgtAdd<<mainWgt;
    btnlist<<btnStart<<btnPractice<<btnAdd;

    QWidget* mg = new QWidget(this);
    mg->setStyleSheet("background-color: #022021; color: #e7f7fa");
    //mg->resize(500,500);
    //mg->setMaximumSize(QWIDGETSIZE_MAX,QWIDGETSIZE_MAX);
    oth_hbx = new QHBoxLayout(mg);
    oth_hbx->addWidget(mainWgt);

    //oth_hbx->setAlignment(mainWgt,Qt::AlignLeft);
    //oth_hbx->setSizeConstraint(QLayout::SetMaximumSize);
    m_hbx->addWidget(mg);

    //m_hbx->setAlignment(mg, Qt::AlignLeft);
    but_vbx->setAlignment(lbl,Qt::AlignTop);

}

void MyCentralWidget::btnStartToggled()
{
    if(i!=3){
        btnlist[i]->setEnabled(true);
        btnlist[i]->setChecked(false);
        menulist[i]->setDisabled(true);
    }
    oth_hbx->removeWidget(lst[i]);
    lst[i]->hide();
    i = 0;
    menulist[i]->setEnabled(true);
    oth_hbx->addWidget(lst[i]);
    btnlist[i]->setDisabled(true);
    btnlist[i]->setChecked(true);
    lst[i]->show();
    lst[i]->setFocus();
}

void MyCentralWidget::btnPracticeToggled(){
    if(i!=3){
        btnlist[i]->setEnabled(true);
        btnlist[i]->setChecked(false);
        menulist[i]->setDisabled(true);
    }
    oth_hbx->removeWidget(lst[i]);
    lst[i]->hide();
    i = 1;
    menulist[i]->setEnabled(true);
    oth_hbx->addWidget(lst[i]);
    btnlist[i]->setDisabled(true);
    btnlist[i]->setChecked(true);
    lst[i]->show();
    lst[i]->setFocus();
}

void MyCentralWidget::btnAddToggled()
{
    if(i!=3){
        btnlist[i]->setEnabled(true);
        btnlist[i]->setChecked(false);
        menulist[i]->setDisabled(true);
    }
    oth_hbx->removeWidget(lst[i]);
    lst[i]->hide();
    i = 2;
    menulist[i]->setEnabled(true);
    oth_hbx->addWidget(lst[i]);
    btnlist[i]->setDisabled(true);
    btnlist[i]->setChecked(true);
    lst[i]->show();
    lst[i]->setFocus();
}


DataBase::~DataBase(){
    this->closeDataBase();
}
