#include "database.h"
#include <QSql>
#include <QSqlError>
#include <QFile>
#include <QMessageBox>
#include <QSqlQuery>
#include <QVariantList>
#include <QSqlRecord>
#include <QDebug>

DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

void DataBase::createConnection(){
    if(!QFile(DATABASE_NAME).exists()){
        if(!restoreDataBase())
            exit(1);
    }else{
        if(!openDataBase())
            exit(1);
    }
}

bool DataBase::openDataBase(){
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName(DATABASE_NAME);
    if(db.open()){
        return true;
    }
    QMessageBox::information(0,"DataBase", "Не удалось открыть базу данных");
    return false;
}

bool DataBase::restoreDataBase(){
    if(openDataBase()){
        if(createTable() && setStartingValues()){
            return true;
        }
    }
    QMessageBox::information(0,"DataBase", "Не удалось создать базу данных");
    return false;
}

void DataBase::closeDataBase(){
    db.close();
}

bool DataBase::createTable(){
    QSqlQuery query;
    if(query.exec("CREATE TABLE " TABLE " ( "
                                "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                TABLE_ENG " VARCHAR(50) NOT NULL, "
                                TABLE_RUS " VARCHAR(50) NOT NULL)")
       && query.exec("CREATE TABLE " SETTINGS_TABLE " ( "
                                SETTINGS_CURPAGE " INTEGER NOT NULL, "
                                SETTINGS_MAXPAGE " INTEGER NOT NULL)")
       && query.exec("CREATE TABLE " PAGE_TABLE " ( "
                                PAGE_ID " INTEGER NOT NULL)"
                     )){
        return true;
    }
    return false;
}

bool DataBase::setStartingValues(){
    QSqlQuery query;
    QVariantList data;
    data<<"English"<<"Английский";
    if(query.exec("INSERT INTO " SETTINGS_TABLE " VALUES (0, 0)")
            && insertIntoPageTable(1) && insertIntoTable(data))
    {
        return true;
    }
    return false;
}

bool DataBase::insertIntoPageTable(int i){
    QSqlQuery query;
    query.prepare("INSERT INTO " PAGE_TABLE
                          " VALUES (:RecNumber)"
                  );
    query.bindValue(":RecNumber", i);
    if(query.exec())
        return true;
    return false;
}

bool DataBase::insertIntoTable(const QVariantList &data){
    QSqlQuery query;
    query.prepare("INSERT INTO " TABLE " ( "
                                TABLE_ENG ", "
                                TABLE_RUS ") "
                          "VALUES (:ENG, :RUS)"
                  );
    query.bindValue(":ENG", data[0].toString());
    query.bindValue(":RUS", data[1].toString());
    if(query.exec())
        return true;
    return false;
}

bool DataBase::updateRecord(const QVariantList &data){
    QSqlQuery query;
    query.prepare("UPDATE " TABLE " SET "
                            TABLE_ENG " = :ENG, "
                            TABLE_RUS " = :RUS "
                            "WHERE id = :id"

                  );
    query.bindValue(":id",  data[0].toInt());
    query.bindValue(":ENG", data[1].toString());
    query.bindValue(":RUS", data[2].toString());
    if(query.exec()){
        return true;
    }
    return false;
}

void DataBase::updateSettings(int cur, int max){
    QSqlQuery query;
    query.prepare("UPDATE " SETTINGS_TABLE " SET "
                            SETTINGS_CURPAGE " = :CurPage, "
                            SETTINGS_MAXPAGE " = :MaxPage"
                            //"WHERE " SETTINGS_CURPAGE " = :OldCur"
                  );

    query.bindValue(":CurPage",  cur);
    query.bindValue(":MaxPage",  max);
    //query.bindValue(":OldCur", old_cur);
    if(!query.exec()){
        QMessageBox::information(0,"EnglishSettings", trUtf8("Ошибка обновления данных из базы"));
        exit(1);
    }



}

bool DataBase::updatePageTable(int old, int cur){
    QSqlQuery query;
    query.prepare("UPDATE " PAGE_TABLE " SET "
                            PAGE_ID " = :RecNumber "
                            "WHERE RecNumber = :prev"

                  );
    query.bindValue(":RecNumber",  cur);
    query.bindValue(":prev", old);
    if(query.exec()){
        return true;
    }
    return false;
}


int DataBase::getRecCount(){
    QSqlQuery query;
    if(!query.exec("SELECT count(*) FROM " TABLE)){
        QMessageBox::information(0,"DataBase", trUtf8("Ошибка выборки данных из базы"));
        exit(1);
    }else{
        query.next();
        return query.value(0).toInt();
    }
}

int DataBase::getFirstNumOfRecord(int n){
    QSqlQuery query;
    if(!query.exec("SELECT min(id) FROM " TABLE " WHERE id > " + QString::number(n) )){
        QMessageBox::information(0, "DataBase", trUtf8("Ошибка выбора данных из базы"));
        exit(1);
    }else{
        query.next();
        return query.value(0).toInt();
    }
}

int DataBase::getLastNumOfRecord(int n){
    QSqlQuery query;
    if(n == 0){
        if(!query.exec("SELECT max(id) FROM " TABLE)){
            QMessageBox::information(0, "DataBase", trUtf8("Ошибка выбора данных из базы"));
            exit(1);
        }else{
            query.next();
            return query.value(0).toInt();
        }
    }else{
        if(!query.exec("SELECT max(id) FROM " TABLE " WHERE id < " + QString::number(n))){
            QMessageBox::information(0, "DataBase", trUtf8("Ошибка выбора данных из базы"));
            exit(1);
        }else{
            query.next();
            return query.value(0).toInt();
        }
    }
}

void DataBase::selectRecords(QVariantList& data, int cur, int last)
{    
    QSqlQuery query;
    if(last){
        if(!query.exec("SELECT " TABLE_ENG ", " TABLE_RUS
                      " FROM " TABLE " WHERE id between " + QString::number(cur) + " and " + QString::number(last)))
        {
            QMessageBox::information(0, "DataBase", trUtf8("Ошибка выбора данных из базы"));
            exit(1);
        }else{
            while(query.next()){
                data << query.value(0) << query.value(1);
            }
        }
    }else{
        if(!query.exec("SELECT " TABLE_ENG ", " TABLE_RUS
                      " FROM " TABLE " WHERE id >= " + QString::number(cur)))
        {
            QMessageBox::information(0, "DataBase", trUtf8("Ошибка выбора данных из базы"));
            exit(1);
        }else{
            while(query.next()){
                data << query.value(0) << query.value(1);
            }
        }
    }
}

void DataBase::selectPageTable(QVector<int> &data){
    QSqlQuery query;
    if(!query.exec("SELECT* FROM " PAGE_TABLE)){
        QMessageBox::information(0, "EnglishPage", trUtf8("Ошибка выбора данных из базы"));
        exit(1);
    }else{
        while(query.next()){
            data<<query.value(0).toInt();
        }
    }
}

int DataBase::getCurrentPage(){
    QSqlQuery query;
    if(!query.exec("SELECT " SETTINGS_CURPAGE " FROM " SETTINGS_TABLE)){
        QMessageBox::information(0, "EnglishSettings", trUtf8("Ошибка выбора данных из базы"));
        exit(1);
    }else{
        query.next();
        return query.value(0).toInt();
    }
}

int DataBase::getMaxPage(){
    QSqlQuery query;
    if(!query.exec("SELECT " SETTINGS_MAXPAGE " FROM " SETTINGS_TABLE)){
        QMessageBox::information(0, "EnglishSettings", trUtf8("Ошибка выбора данных из базы"));
        exit(1);
    }else{
        query.next();
        return query.value(0).toInt();
    }
}

void DataBase::deleteFromPageTables(int num){
    QSqlQuery query;
    if(!query.exec("DELETE FROM " PAGE_TABLE " WHERE " PAGE_ID " = " + QString::number(num))){
        QMessageBox::information(0,"EnglishPage", trUtf8("Ошибка удаления данных из базы"));
        exit(1);
    }    
}

const QString DataBase::selectOneRecord(int num, QStringList& lst){
    QSqlQuery query;
    int n = getFirstNumOfRecord(num);
    if(!query.exec("SELECT " TABLE_ENG ", " TABLE_RUS
                   " FROM " TABLE " WHERE id = " + QString::number(n))){
        QMessageBox::information(0, "DataBase", trUtf8("Ошибка выбора данных из базы"));
        exit(1);
    }else{
        query.next();
        lst<<query.value(1).toString();
        return query.value(0).toString();
    }
}

int DataBase::getPageCount(){
    QSqlQuery query;
    if(!query.exec("SELECT count(*) FROM " PAGE_TABLE)){
        QMessageBox::information(0,"EnglishPage", trUtf8("Ошибка выборки данных из базы"));
        exit(1);
    }else{
        query.next();
        return query.value(0).toInt();
    }
}
