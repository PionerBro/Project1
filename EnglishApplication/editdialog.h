#ifndef EDITDIALOG_H
#define EDITDIALOG_H

#include <QDialog>

class QPushButton;
class QLineEdit;
class QSqlRecord;

class EditDialog : public QDialog
{
    Q_OBJECT
public:
    EditDialog(const QSqlRecord &rec, QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
private:
    QPushButton* btnOk;
    QLineEdit* englne;
    QLineEdit* ruslne;
    int row;
private slots:
    void updateToDataBase();
    void btnOkEnable();
signals:
    void updateDataBase(const QVariantList&);
};

#endif // EDITDIALOG_H
