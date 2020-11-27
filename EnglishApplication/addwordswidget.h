#ifndef ADDWORDSWIDGET_H
#define ADDWORDSWIDGET_H

#include <QWidget>
#include <QSqlTableModel>

class DataBase;
class QMenu;
class QTableView;


class AddWordsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AddWordsWidget(DataBase* db, QWidget *parent = nullptr);
    void setMenuActions(QMenu* menu);
private:
    int db_recCount;
    QTableView* view;
    DataBase* m_db;
    QSqlTableModel* model;
    QAction* addAct;
    QAction* editAct;
    QAction* deleteAct;
private:
    void setupModel(const QString& tableName, const QStringList& headers);
    void addView();
private slots:
    void btnAddClicked();
    void btnEditClicked();
    void btnDeleteClicked();
    void addWords(const QVariantList& data);
    void editWords(const QVariantList& data);
signals:
    //void addRecIntoPageTable();
    void signalEdit(int);
    void signalDelete(int);
    void signalForLearnWidget();
};

#endif // ADDWORDSWIDGET_H
