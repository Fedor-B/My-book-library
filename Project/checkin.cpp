#include "checkin.h"
#include "ui_checkin.h"

#include <QFile>
#include <QMessageBox>


Crypter strcryp;
QFile filepas("../projectOne/filepassword.txt");
QFile filelog("../projectOne/filelogin.txt");


CheckIn::CheckIn(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CheckIn)
{
    ui->setupUi(this);
     ui->pasreg1LineEdit->setEchoMode(QLineEdit::Password);
     ui->pasreg2LineEdit->setEchoMode(QLineEdit::Password);
}

CheckIn::~CheckIn()
{
    delete ui;
}

void CheckIn::on_pushButton_clicked()
{
    if(loginCryp()&&passwordCryp())
    {
        Authorization *aut = new Authorization();
        aut->setWindowTitle(trUtf8("Вход"));
        aut->show();
        this->close();
    }
}

int  CheckIn::loginCryp()
{

QString str;
    if(ui->logregLineEdit->text()!=nullptr)
    {
        if(filelog.open(QIODevice::WriteOnly))
            {
                str = strcryp.cryptString(ui->logregLineEdit->text());
                filelog.write(str.toUtf8());
                filelog.close();
            }
            else
                qDebug()<<"error logFile";
    }
    else
    {
        QMessageBox::critical(this,"Внимание!","Строка с логином пуста!");
        return 0;
    }
    return 1;
}

int CheckIn::passwordCryp()
{

    if(ui->pasreg1LineEdit->text()==ui->pasreg2LineEdit->text())
    {
        if(filepas.open(QIODevice::WriteOnly))
        {
            QString str = strcryp.cryptString(ui->pasreg1LineEdit->text());
            filepas.write(str.toUtf8());
            filepas.close();
        }
        else
            qDebug()<<"error pasFile";
    }
    else
    {
        QMessageBox::critical(this,"Внимание!","Пароли не совпадают!");
        return 0;
    }
    return 1;
}

QString CheckIn::loginDecryp()
{
    if(filelog.open(QIODevice::ReadOnly))
    {
        QString str = strcryp.decryptString(filelog.readAll());
        filelog.close();
        return str;
    }
    else
    {
        qDebug()<<"error Decryp login!";
        return nullptr;
    }
}

QString CheckIn::passwordDecryp()
{
    if(filepas.open(QIODevice::ReadOnly))
    {
        QString str = strcryp.decryptString(filepas.readAll());
        filepas.close();
        return str;
    }
    else
    {
        qDebug()<<"error Decryp password!";
        return nullptr;
    }
}

bool CheckIn::loginRead()
{
    if(filelog.open(QIODevice::ReadOnly))
    {
        QString str = filelog.readAll();
        filelog.close();
        if(str!=nullptr)
            return true;
    }
    return false;
}

void CheckIn::loginRemove()
{
   if (filelog.open(QIODevice::WriteOnly | QIODevice::Truncate))
      filelog.close();
}

void CheckIn::passwordRemove()
{
    if (filepas.open(QIODevice::WriteOnly | QIODevice::Truncate))
      filepas.close();
}
