#ifndef AUTHORIZATION_H
#define AUTHORIZATION_H

#include <QDialog>
#include "mainwindow.h"
#include "Crypter.h"
#include "guestclass.h"
#include "checkin.h"

namespace Ui {
class Authorization;
}

class Authorization : public QDialog
{
    Q_OBJECT

public:
    explicit Authorization(QWidget *parent = nullptr);
    ~Authorization();

private slots:
    void on_inputButton_clicked();

    void on_regButton_clicked();


    void on_guestButton_clicked();

private:
    Ui::Authorization *ui;
};

#endif // AUTHORIZATION_H
