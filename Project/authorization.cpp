#include "authorization.h"
#include "ui_authorization.h"



#include <QMessageBox>

bool fl ;
Crypter str;

Authorization::Authorization(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Authorization)
{
    ui->setupUi(this);

    CheckIn check;
    fl = check.loginRead();
    if(fl == true)
    {
        ui->regButton->setEnabled(false);
    }
    ui->passwordLineEdit->setEchoMode(QLineEdit::Password);
}

Authorization::~Authorization()
{
    delete ui;
}

void Authorization::on_inputButton_clicked()
{
    CheckIn string;
    if(ui->loginLineEdit->text()==string.loginDecryp()&&ui->passwordLineEdit->text()==string.passwordDecryp())
    {

        MainWindow *MyLibrary = new MainWindow();
        MyLibrary->setWindowTitle(trUtf8("Библиотека"));
        MyLibrary->show();
        this->close();
    }
    else
    {
        QMessageBox::critical(this,"Внимание!","Неверный логин/пароль!");
        qDebug()<<string.loginDecryp()+"   "+string.passwordDecryp();
    }
}

void Authorization::on_regButton_clicked()
{
    CheckIn *check = new CheckIn();
    check->setWindowTitle(trUtf8("Регистрация"));
    check->show();
    fl = true;
    this->close();

}



void Authorization::on_guestButton_clicked()
{
    GuestClass *view = new GuestClass();
    view->setWindowTitle(trUtf8("Просмотр библиотеки"));
    view->show();
    this->close();
}
