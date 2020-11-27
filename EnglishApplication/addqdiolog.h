#ifndef ADDQDIOLOG_H
#define ADDQDIOLOG_H


#include <QDialog>

class QPushButton;
class QLineEdit;


class AddQDialog : public QDialog
{
    Q_OBJECT
public:
    AddQDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowFlags());
private:
    QPushButton* btnOk;
    QLineEdit* englne;
    QLineEdit* ruslne;
private slots:
    void insertIntoDataBase();
    void btnOkEnable();
signals:
    void addToDataBase(const QVariantList&);
};

#endif // ADDQDIOLOG_H
