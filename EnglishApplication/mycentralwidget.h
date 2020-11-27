#ifndef MYCENTRALWIDGET_H
#define MYCENTRALWIDGET_H

#include <QWidget>

class QPushButton;
class QHBoxLayout;
class QMainWindow;
class DataBase;
class QMenu;
class LearnWidget;
class ExerciseWidget;
class AddWordsWidget;


class MyCentralWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MyCentralWidget(QList<QMenu*>& list, QWidget *parent = nullptr);
private:
    DataBase *db;
    //QPushButton* btnStart;
    //QPushButton* btnAdd;
    //QPushButton* btnPractice;
    //QWidget* wgtStart;
    //QWidget* wgtExc;
    //QWidget* wgtAdd;
    LearnWidget* wgtStart;
    ExerciseWidget* wgtExc;
    AddWordsWidget* wgtAdd;



    QHBoxLayout* oth_hbx;
    QHBoxLayout* hbxStart;
    QHBoxLayout* hbxAdd;
    QWidget* mainWgt;
    QList<QWidget*> lst;
    QList<QPushButton*> btnlist;
    QList<QMenu* > menulist;

    int i = 3;
private slots:
    void btnStartToggled();
    void btnAddToggled();
    void btnPracticeToggled();
signals:


};

#endif // MYCENTRALWIDGET_H
