#ifndef CHECKIN_H
#define CHECKIN_H

#include <QDialog>
#include "authorization.h"
#include "Crypter.h"
namespace Ui {
class CheckIn;
}

class CheckIn : public QDialog
{
    Q_OBJECT

public:
    explicit CheckIn(QWidget *parent = nullptr);
    ~CheckIn();
    QString loginDecryp();
    QString passwordDecryp();
    bool loginRead();
    void loginRemove();
    void passwordRemove();
private slots:
    void on_pushButton_clicked();



    int loginCryp();
    int passwordCryp();


private:
    Ui::CheckIn *ui;
};

#endif // CHECKIN_H
