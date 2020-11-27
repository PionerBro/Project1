#ifndef EXERCISEWIDGET_H
#define EXERCISEWIDGET_H

#include <QWidget>

class DataBase;
class QLabel;
class QLineEdit;
class QPushButton;
class QMenu;

class ExerciseWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ExerciseWidget(DataBase *db, QWidget *parent = nullptr);
    void setMenuActions(QMenu* menu);
private:
    QPushButton* btnCheck;
    DataBase* m_db;
    QList<QLabel*> reslist;
    QList<QLabel*> lbllist;
    QList<QLineEdit*> lnelist;
    QStringList answerlist;
    QAction* startAct;
    QAction* checkAct;
private slots:
    void slotStart();
    void slotCheck();
signals:

};

#endif // EXERCISEWIDGET_H
