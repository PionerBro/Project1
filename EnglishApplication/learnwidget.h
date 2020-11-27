#ifndef LEARNWIDGET_H
#define LEARNWIDGET_H

#include <QWidget>

class DataBase;
class QLabel;
class QPushButton;
class QMenu;

class LearnWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LearnWidget(DataBase *db, QWidget *parent = nullptr);
    void setMenuActions(QMenu* menu);
private:
    int db_recCount;
    int cur_page;
    int max_page;
    QVector<int> page_table;
    DataBase* m_db;
    QList<QLabel*> engLabelList;
    QList<QLabel*> rusLabelList;
    QList<QLabel*> sepLabelList;
    QPushButton* btnNext;
    QPushButton* btnPrev;
    QAction* nextAct;
    QAction* prevAct;
private:
    void fillPage();
private slots:
    void nextPage();
    void prevPage();
    void updateLearnWidget();
    void updateRecord(int);
    void deleteRecord(int);
signals:
};

#endif // LEARNWIDGET_H
