#include "addwordswidget.h"
#include "addqdiolog.h"
#include "editdialog.h"
#include "database.h"
#include <QDialog>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QTableView>
#include <QPushButton>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QSqlRecord>
#include <QMenu>
#include <QDebug>


AddWordsWidget::AddWordsWidget(DataBase* db, QWidget *parent) : QWidget(parent), m_db(db)
{
    //db_recCount = m_db->getRecCount();
    QHBoxLayout* hbx = new QHBoxLayout(this);
    hbx->setContentsMargins(0,0,0,0);
    view = new QTableView(this);
    hbx->addWidget(view);
    QVBoxLayout* vbx = new QVBoxLayout;
    hbx->addLayout(vbx);
    QPushButton* btnAdd = new QPushButton(trUtf8("Добавить"),this);
    QPushButton* btnEdit = new QPushButton(trUtf8("Редактировать"), this);
    QPushButton* btnDelete = new QPushButton(trUtf8("Удалить"),this);
    btnAdd->setFocusPolicy(Qt::NoFocus);
    btnEdit->setFocusPolicy(Qt::NoFocus);
    btnDelete->setFocusPolicy(Qt::NoFocus);
    vbx->addWidget(btnAdd);
    vbx->addWidget(btnEdit);
    vbx->addWidget(btnDelete);
    vbx->setAlignment(Qt::AlignVCenter);
    connect(btnAdd, SIGNAL(clicked()), this, SLOT(btnAddClicked()));
    connect(btnEdit, SIGNAL(clicked()), this, SLOT(btnEditClicked()));
    connect(btnDelete, SIGNAL(clicked()), this, SLOT(btnDeleteClicked()));
    addAct = new QAction(trUtf8("Добавить"),this);
    editAct = new QAction(trUtf8("Редактировать"),this);
    deleteAct = new QAction(trUtf8("Удалить"), this);
    addAct->setShortcut(trUtf8("Insert"));
    editAct->setShortcut(trUtf8("F4"));
    deleteAct->setShortcut(trUtf8("Delete"));
    addAct->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    editAct->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    deleteAct->setShortcutContext(Qt::WidgetWithChildrenShortcut);
    this->addAction(addAct);
    this->addAction(editAct);
    this->addAction(deleteAct);
    connect(addAct, SIGNAL(triggered()), this, SLOT(btnAddClicked()));
    connect(editAct, SIGNAL(triggered()), this, SLOT(btnEditClicked()));
    connect(deleteAct, SIGNAL(triggered()), this, SLOT(btnDeleteClicked()));
    this->setupModel(TABLE, QStringList() << trUtf8("id")
                                          << trUtf8("Английский")
                                          << trUtf8("Русский"));
    this->addView();
}

void AddWordsWidget::setupModel(const QString& tableName, const QStringList& headers){
    model = new QSqlTableModel(this);
    model->setTable(tableName);
    for(int i=0; i<model->columnCount(); ++i){
        model->setHeaderData(i, Qt::Horizontal, headers[i]);
    }
    model->setSort(0, Qt::AscendingOrder);
}

void AddWordsWidget::addView(){
    view->setModel(model);
    view->setColumnHidden(0, true);
    view->setSelectionBehavior(QAbstractItemView::SelectRows);
    view->setSelectionMode(QAbstractItemView::SingleSelection);
    view->resizeColumnsToContents();
    view->setEditTriggers(QAbstractItemView::NoEditTriggers);
    view->horizontalHeader()->setStretchLastSection(true);
    model->select();


    //view->addAction(addAct);

    //view->addAction(editAct);
    //view->addAction(deleteAct);
}

void AddWordsWidget::btnAddClicked(){
     AddQDialog *dialog = new AddQDialog(this);
     connect(dialog, SIGNAL(addToDataBase(const QVariantList&)), this, SLOT(addWords(const QVariantList&)));
}

void AddWordsWidget::addWords(const QVariantList& data){
    if(m_db->insertIntoTable(data)){
        model->select();
        view->selectRow(model->rowCount()-1);
        emit signalForLearnWidget();
        //if((++db_recCount)%20==1){
        //    emit addRecIntoPageTable();
        //}
    }else{
        QMessageBox::information(0, trUtf8("DataBase"), trUtf8("Ну удалось добавить данные в базу"));
    }
}

void AddWordsWidget::btnEditClicked(){
    if(view->currentIndex().row() != -1){
        QSqlRecord rec = model->record(view->currentIndex().row());
        if(rec.value(0).toInt()){
            EditDialog* dialog = new EditDialog(rec, this);
            connect(dialog, SIGNAL(updateDataBase(const QVariantList&)), this, SLOT(editWords(const QVariantList&)));
        }
    }
}

void AddWordsWidget::editWords(const QVariantList& data){
    if(m_db->updateRecord(data)){
        emit signalEdit(data[0].toInt());
        int row = view->currentIndex().row();
        model->select();
        view->selectRow(row);
    }else{
        QMessageBox::information(0, trUtf8("DataBase"), trUtf8("Ну удалось добавить данные в базу"));
    }
}

void AddWordsWidget::btnDeleteClicked(){
    if(view->currentIndex().row() != -1){
        QSqlRecord rec = model->record(view->currentIndex().row());
        if(rec.value(0).toInt()){
            int row = view->currentIndex().row();
            int num = model->record(view->currentIndex().row()).value(0).toInt();
            model->removeRow(view->currentIndex().row());
            model->select();
            if(row < model->rowCount())
                view->selectRow(row);
            else{
                if(row)
                    view->selectRow(row-1);
            }
            emit signalDelete(num);
        }
    }
}

void AddWordsWidget::setMenuActions(QMenu *menu){
    menu->addAction(addAct);
    menu->addAction(editAct);
    menu->addAction(deleteAct);
}
