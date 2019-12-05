#ifndef DIALOGBOOKPRIVATE_H
#define DIALOGBOOKPRIVATE_H

#include <QSqlTableModel>
#include <QDialog>
#include <databaseprivate.h>
#include <QDataWidgetMapper>
#include <QAbstractButton>

namespace Ui {
class DialogBookPrivate;
}

class DialogBookPrivate : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBookPrivate(int row, QWidget *parent = nullptr);
    ~DialogBookPrivate();

signals:
    void signalReady();

private slots:
    void updateButtons(int row);
    void on_buttonBox_accepted();

private:
    Ui::DialogBookPrivate *ui;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;
    void setupModel();
    void accept();

};
#endif // DIALOGBOOKPRIVATE_H
