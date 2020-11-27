#include "editdialog.h"
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QRegExp>
#include <QVariantList>
#include <QSqlRecord>

EditDialog::EditDialog(const QSqlRecord &rec, QWidget* parent, Qt::WindowFlags f):QDialog(parent, f)
{
    this->setModal(true);
    this->setAttribute(Qt::WA_DeleteOnClose);
    QVBoxLayout* addvbx = new QVBoxLayout(this);
    QLabel* englbl = new QLabel(trUtf8("Английский"),this);
    QLabel* ruslbl = new QLabel(trUtf8("Русский"),this);
    QHBoxLayout* addLabelhbx = new QHBoxLayout;
    addLabelhbx->addWidget(englbl);
    addLabelhbx->addWidget(ruslbl);
    englne = new QLineEdit(this);
    ruslne = new QLineEdit(this);
    row = rec.value(0).toInt();
    englne->setText(rec.value(1).toString());
    ruslne->setText(rec.value(2).toString());
    QHBoxLayout* addEdithbx = new QHBoxLayout;
    addEdithbx->addWidget(englne);
    addEdithbx->addWidget(ruslne);
    btnOk = new QPushButton(trUtf8("Добавить"),this);
    btnOk->setDisabled(true);
    QPushButton* btnCancel = new QPushButton(trUtf8("Отмена"), this);
    QHBoxLayout* addButtonhbx = new QHBoxLayout;
    addButtonhbx->addWidget(btnOk);
    addButtonhbx->addWidget(btnCancel);
    addvbx->addLayout(addLabelhbx);
    addvbx->addLayout(addEdithbx);
    addvbx->addLayout(addButtonhbx);
    connect(englne, SIGNAL(textEdited(const QString&)), this, SLOT(btnOkEnable()));
    //connect(englne, SIGNAL(returnPressed()), this, SLOT(insertIntoDataBase()));
    connect(ruslne, SIGNAL(textEdited(const QString&)), this, SLOT(btnOkEnable()));
    //connect(ruslne, SIGNAL(returnPressed()), this, SLOT(insertIntoDataBase()));
    connect(btnOk, SIGNAL(clicked()), this, SLOT(updateToDataBase()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    this->show();
}

void EditDialog::updateToDataBase(){
    QString eng = englne->text();
    QString rus = ruslne->text();
    if((eng.isEmpty()) || (rus.isEmpty()))
        return;
    if(eng.toUpper().contains(QRegExp("[^-A-Z\\s]")) ||
            rus.toUpper().contains(QRegExp("[^-А-Я\\s]")))
    {
        QMessageBox::information(0, trUtf8("Слова"), trUtf8("Вы ввели неверные данные"));
        return;
    }
    QVariantList data;
    data<<row<<eng<<rus;
    emit updateDataBase(data);
    this->close();
}

void EditDialog::btnOkEnable(){
    if((!englne->text().isEmpty()) && (!ruslne->text().isEmpty())){
        btnOk->setEnabled(true);
    }else{
        btnOk->setDisabled(true);
    }
}


