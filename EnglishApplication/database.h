#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>

#define DATABASE_HOSTNAME   "EnglishApplication"
#define DATABASE_NAME       "English.db"

#define TABLE       "English"
#define TABLE_ENG    "ENG"
#define TABLE_RUS    "RUS"

#define SETTINGS_TABLE "EnglishSettings"
#define SETTINGS_CURPAGE  "CurPage"
#define SETTINGS_MAXPAGE  "MaxPage"

#define PAGE_TABLE   "EnglishPage"
#define PAGE_ID      "RecNumber"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();
    void createConnection();
    bool insertIntoTable(const QVariantList& data);
    bool insertIntoPageTable(int i);
    bool updateRecord(const QVariantList& data);
    void updateSettings(int cur, int max);
    bool updatePageTable(int old, int cur);
    void selectRecords(QVariantList& data, int cur, int last = 0);
    void selectPageTable(QVector<int>& data);
    int getCurrentPage();
    int getMaxPage();
    int getRecCount();
    int getLastNumOfRecord(int n = 0);
    int getFirstNumOfRecord(int n = 0);
    void deleteFromPageTables(int num);
    const QString selectOneRecord(int num, QStringList& lst);
private:
    QSqlDatabase db;
private:
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTable();
    bool setStartingValues();
signals:

};

#endif // DATABASE_H
