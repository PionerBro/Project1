#include "exercisewidget.h"
#include "database.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QRandomGenerator>
#include <QDebug>
#include <QMenu>

ExerciseWidget::ExerciseWidget(DataBase* db, QWidget *parent) : QWidget(parent), m_db(db)
{
    this->setStyleSheet("QPushButton {background-color: #34051b}"
                        "QLabel {color: #ffbf1b}"
                        "QLineEdit {border: 1px solid #000000; color: #ffbf1b}");

    QVBoxLayout* vbx = new QVBoxLayout(this);
    QHBoxLayout* hbx1 = new QHBoxLayout;
    QHBoxLayout* hbx2 = new QHBoxLayout;
    vbx->addLayout(hbx1);
    vbx->addLayout(hbx2);
    QVBoxLayout* vbx1 = new QVBoxLayout;
    QVBoxLayout* vbx2 = new QVBoxLayout;
    QVBoxLayout* vbx3 = new QVBoxLayout;
    QVBoxLayout* vbx4 = new QVBoxLayout;
    QVBoxLayout* vbx5 = new QVBoxLayout;
    QVBoxLayout* vbx6 = new QVBoxLayout;
    hbx1->addLayout(vbx1);
    hbx1->addLayout(vbx2);
    hbx1->addLayout(vbx3);
    hbx1->addLayout(vbx4);
    hbx1->addLayout(vbx5);
    hbx1->addLayout(vbx6);
    for (int i= 0; i<20; ++i) {
        //QLabel* lbl = new QLabel(this);
        //QLineEdit* lne = new QLineEdit(this);
        //lbllist<<lbl;
        //lnelist<<lne;
        lbllist<<(new QLabel(this));
        lbllist[i]->setFixedHeight(26);
        lnelist<<(new QLineEdit(this));
        lnelist[i]->setFixedHeight(26);
        lnelist[i]->setDisabled(true);
        reslist<<(new QLabel(this));
        reslist[i]->setFixedHeight(26);
    }
    for(int i = 0; i<10; ++i){
        vbx1->addWidget(lbllist[i]);
        vbx2->addWidget(lnelist[i]);
        vbx3->addWidget(reslist[i]);
        vbx4->addWidget(lbllist[i+10]);
        vbx5->addWidget(lnelist[i+10]);
        vbx6->addWidget(reslist[i+10]);
    }
    QPushButton* btnStart = new QPushButton(trUtf8("Практиковаться"), this);
    btnStart->setFocusPolicy(Qt::NoFocus);
    btnCheck = new QPushButton(trUtf8("Проверить"), this);
    btnCheck->setDisabled(true);
    btnCheck->setFocusPolicy(Qt::NoFocus);
    connect(btnStart, SIGNAL(clicked()), this, SLOT(slotStart()));
    connect(btnCheck, SIGNAL(clicked()), this, SLOT(slotCheck()));
    hbx2->addWidget(btnStart);
    hbx2->addWidget(btnCheck);
    startAct = new QAction(trUtf8("Заполнить"),this);
    checkAct = new QAction(trUtf8("Проверить"),this);
    connect(startAct, SIGNAL(triggered()), this, SLOT(slotStart()));
    connect(checkAct, SIGNAL(triggered()), this, SLOT(slotCheck()));
    startAct->setShortcut(trUtf8("Insert"));
    checkAct->setShortcut(trUtf8("CTRL+R"));
    checkAct->setDisabled(true);
    startAct->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    checkAct->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    this->addAction(startAct);
    this->addAction(checkAct);
}

void ExerciseWidget::slotStart(){
    if(!m_db->getPageCount()){
        return;
    }
    QVector<int> vec;
    answerlist.clear();
    m_db->selectPageTable(vec);
    int page = m_db->getMaxPage();
    int num;
    if(page == (vec.count()-1))
        num = m_db->getLastNumOfRecord();
    else
        num = m_db->getLastNumOfRecord(vec[page+1]);
    for(int i =0; i<20; ++i) {
        lbllist[i]->setText(m_db->selectOneRecord(QRandomGenerator::global()->generate()%num, answerlist));
        lnelist[i]->setEnabled(true);
        lnelist[i]->setToolTip("");
        reslist[i]->clear();
        lnelist[i]->clear();
    }
    btnCheck->setEnabled(true);
    checkAct->setEnabled(true);

}

void ExerciseWidget::slotCheck(){
    btnCheck->setDisabled(true);
    checkAct->setDisabled(true);
    for(int i = 0; i<20; ++i){
        if(answerlist[i].compare(lnelist[i]->text(), Qt::CaseInsensitive)){
            reslist[i]->setText("No");
            reslist[i]->setStyleSheet("color: #8a0000");
        }else{
            reslist[i]->setText("Yes");
            reslist[i]->setStyleSheet("color: #00d400");
        }
        lnelist[i]->setDisabled(true);
        lnelist[i]->setToolTip(answerlist[i]);
        this->setFocus();
    }
}

void ExerciseWidget::setMenuActions(QMenu* menu){
    menu->addAction(startAct);
    menu->addAction(checkAct);
}
