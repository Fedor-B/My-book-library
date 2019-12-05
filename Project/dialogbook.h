#ifndef DIALOGBOOK_H
#define DIALOGBOOK_H

#include <QSqlTableModel>
#include <QDialog>
#include <database.h>
#include <QDataWidgetMapper>
#include <QAbstractButton>

namespace Ui {
class DialogBook;
}

class DialogBook : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBook(int row, QWidget *parent = nullptr);
    ~DialogBook();

signals:
    void signalReady();

private slots:
    void updateButtons(int row);
    void on_buttonBox_accepted();

private:
    Ui::DialogBook *ui;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;
    void setupModel();
    void accept();

};

#endif // DIALOGBOOK_H
