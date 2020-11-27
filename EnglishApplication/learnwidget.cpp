#include "learnwidget.h"
#include "database.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>
#include <QMenu>
#include <QSizePolicy>

LearnWidget::LearnWidget(DataBase *db, QWidget *parent) : QWidget(parent), m_db(db)
{
    this->setStyleSheet("QPushButton {background-color: #34051b}"
                        "QLabel {color: #c0ae00}");

    QVBoxLayout* vbx = new QVBoxLayout(this);
    QHBoxLayout* hbx1 = new QHBoxLayout;
    QHBoxLayout* hbx2 = new QHBoxLayout;
    vbx->addLayout(hbx1);
    vbx->addLayout(hbx2);
    for (int i=0; i<20; ++i) {
        //QLabel* lbl = new QLabel(this);
        //QString str = QString::number(i);
        engLabelList<<(new QLabel(this));
        engLabelList[i]->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        sepLabelList<<(new QLabel(trUtf8("-"), this));
        sepLabelList[i]->setMaximumWidth(10);
        rusLabelList<<(new QLabel(this));
    }
    QVBoxLayout* vbx1 = new QVBoxLayout;
    QVBoxLayout* vbx2 = new QVBoxLayout;
    QVBoxLayout* vbx3 = new QVBoxLayout;
    QVBoxLayout* vbx4 = new QVBoxLayout;
    QVBoxLayout* vbx5 = new QVBoxLayout;
    QVBoxLayout* vbx6 = new QVBoxLayout;
    for(int i = 0; i<10; ++i){
        vbx1->addWidget(engLabelList[i]);
        vbx2->addWidget(sepLabelList[i]);
        vbx3->addWidget(rusLabelList[i]);
        vbx4->addWidget(engLabelList[i+10]);
        vbx5->addWidget(sepLabelList[i+10]);
        vbx6->addWidget(rusLabelList[i+10]);
    }
    vbx1->setAlignment(Qt::AlignRight);
    vbx4->setAlignment(Qt::AlignRight);
    hbx1->addLayout(vbx1);
    hbx1->addLayout(vbx2);
    hbx1->addLayout(vbx3);
    QWidget* wgtSeparator = new QWidget(this);
    wgtSeparator->setMaximumWidth(10);
    wgtSeparator->setStyleSheet("background-color: #343839");
    hbx1->addWidget(wgtSeparator);
    hbx1->addLayout(vbx4);
    hbx1->addLayout(vbx5);
    hbx1->addLayout(vbx6);
    btnPrev = new QPushButton("<<", this);
    btnNext = new QPushButton(">>", this);
    hbx2->addWidget(btnPrev);
    hbx2->addWidget(btnNext);
    btnPrev->setFocusPolicy(Qt::NoFocus);
    btnNext->setFocusPolicy(Qt::NoFocus);
    connect(btnNext, SIGNAL(clicked()), this, SLOT(nextPage()));
    connect(btnPrev, SIGNAL(clicked()), this, SLOT(prevPage()));
    nextAct = new QAction(trUtf8("Следующая страница"),this);
    nextAct->setShortcut(trUtf8("Right"));
    nextAct->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    prevAct = new QAction(trUtf8("Предыдущая страница"), this);
    prevAct->setShortcut(trUtf8("Left"));
    prevAct->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    this->addAction(nextAct);
    this->addAction(prevAct);
    connect(nextAct, SIGNAL(triggered()), this, SLOT(nextPage()));
    connect(prevAct, SIGNAL(triggered()), this, SLOT(prevPage()));

    db_recCount = db->getRecCount();
    cur_page = m_db->getCurrentPage();
    max_page = m_db->getMaxPage();
    m_db->selectPageTable(page_table);

    this->fillPage();
}



void LearnWidget::fillPage(){
    if(page_table.count()==0){
        for (int i=0; i<20 ;++i) {
            engLabelList[i]->clear();
            rusLabelList[i]->clear();
        }
        btnNext->setDisabled(true);
        nextAct->setDisabled(true);
        btnPrev->setDisabled(true);
        prevAct->setDisabled(true);
        return;
    }
    QVariantList data;    
    //qDebug()<<cur_page<<page_table[cur_page];
    if(cur_page == page_table.count()-1){
        m_db->selectRecords(data, page_table[cur_page]);
        btnNext->setDisabled(true);
        nextAct->setDisabled(true);
    }else{
        m_db->selectRecords(data, page_table[cur_page], page_table[cur_page+1]-1);
        btnNext->setEnabled(true);
        nextAct->setEnabled(true);
    }
    int i = 0;
    int j = 0;
    int data_count = data.count()/2;
    for(; i<data_count; ++i){
        engLabelList[i]->setText(data[j++].toString());
        rusLabelList[i]->setText(data[j++].toString());
    }
    for (; i<20 ;++i) {
        engLabelList[i]->clear();
        rusLabelList[i]->clear();
    }
    if(cur_page==0){
        btnPrev->setDisabled(true);
        prevAct->setDisabled(true);
    }else{
        btnPrev->setEnabled(true);
        prevAct->setEnabled(true);
    }    
}

void LearnWidget::nextPage(){
    ++cur_page;
    max_page = (cur_page > max_page) ? cur_page : max_page;
    m_db->updateSettings(cur_page, max_page);
    fillPage();
    /*btnPrev->setEnabled(true);
    m_id = p_id+1;
    fillPage();*/
}

void LearnWidget::prevPage(){
    --cur_page;
    m_db->updateSettings(cur_page, max_page);
    fillPage();
}

void LearnWidget::updateLearnWidget(){
    if((++db_recCount)%20==1){
        if(cur_page == page_table.count()-1){
            btnNext->setEnabled(true);
            nextAct->setEnabled(true);
        }
        int rec = m_db->getLastNumOfRecord();
        page_table<<rec;
        m_db->insertIntoPageTable(rec);        
    }
    if(cur_page == page_table.count()-1){
        fillPage();
    }
}

void LearnWidget::updateRecord(int num){
    if(num>=page_table[cur_page]){
        if(cur_page<(page_table.count()-1)){
            if(num<page_table[cur_page+1])
                fillPage();
        }else{
            fillPage();
        }
    }
}

void LearnWidget::deleteRecord(int num){

    --db_recCount;
    if(num > (m_db->getLastNumOfRecord()) && num == page_table[page_table.count()-1]){
        if(max_page == page_table.count()-1)
            --max_page;
        if(cur_page == (page_table.count()-1)){
            if(cur_page!=0){
                --cur_page;
                m_db->updateSettings(cur_page, max_page);
            }
            fillPage();
        }

        page_table.pop_back();
        m_db->deleteFromPageTables(num);
        if(cur_page == page_table.count()-1){
            btnNext->setDisabled(true);
            nextAct->setDisabled(true);
        }
    }else{
        int i = page_table.count()-1;
        for(; i >= 0; --i){
            if(num>page_table[i]){
                break;
            }
        }
        if(num == page_table[0] && db_recCount==0){
            page_table.pop_back();
            m_db->deleteFromPageTables(num);
        }else{
            if(!(db_recCount%20)){
                if(max_page == (page_table.count()-1)){
                    --max_page;
                    if(cur_page == (page_table.count()-1))
                        --cur_page;
                    m_db->updateSettings(cur_page, max_page);
                }
                m_db->deleteFromPageTables(page_table[page_table.count()-1]);
                page_table.pop_back();

            }
            int a = i++;
            for(; i<page_table.count(); ++i){
                int cur = m_db->getFirstNumOfRecord(page_table[i]);
                m_db->updatePageTable(page_table[i],cur);
                page_table[i] = cur;
            }
            if(cur_page>=a)
                fillPage();
        }
        if(cur_page==0){
            btnPrev->setDisabled(true);
            prevAct->setDisabled(true);
        }
    }
}

void LearnWidget::setMenuActions(QMenu* menu){
    menu->addAction(nextAct);
    menu->addAction(prevAct);
}
