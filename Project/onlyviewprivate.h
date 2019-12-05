#ifndef ONLYVIEWPRIVATE_H
#define ONLYVIEWPRIVATE_H

#include <databaseprivate.h>
#include <QDataWidgetMapper>
#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class OnlyViewPrivate;
}

class OnlyViewPrivate : public QDialog
{
    Q_OBJECT

public:
    explicit OnlyViewPrivate(int row, QWidget *parent = nullptr);
    ~OnlyViewPrivate();

signals:
    void signalReady();

private slots:
    void updateButtons(int row);
private:
    Ui::OnlyViewPrivate *ui;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;
    void accept();
    void setupModel();
    void on_buttonBox_accepted();
};

#endif // ONLYVIEWPRIVATE_H
