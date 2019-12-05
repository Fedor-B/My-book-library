#ifndef GUESTCLASS_H
#define GUESTCLASS_H

#include <QDialog>
#include <QSqlTableModel>
#include "database.h"

#include "authorization.h"
#include "onlyview.h"

namespace Ui {
class GuestClass;
}

class GuestClass : public QDialog
{
    Q_OBJECT

public:
    explicit GuestClass(QWidget *parent = nullptr);
    ~GuestClass();
    DataBase        *db;
    QSqlTableModel  *model;
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();
private slots:
    void on_exitButton_clicked();


    void slotOpenView();


private:
    Ui::GuestClass *ui;
};

#endif // GUESTCLASS_H
