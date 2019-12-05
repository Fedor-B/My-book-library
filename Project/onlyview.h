#ifndef ONLYVIEW_H
#define ONLYVIEW_H

#include <database.h>
#include <QDataWidgetMapper>
#include <QDialog>
#include <QSqlTableModel>

namespace Ui {
class OnlyView;
}

class OnlyView : public QDialog
{
    Q_OBJECT

public:
    explicit OnlyView(int row,QWidget *parent = nullptr);
    ~OnlyView();
signals:
    void signalReady();

private slots:
    void updateButtons(int row);
private:
    Ui::OnlyView *ui;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;
    void accept();
    void setupModel();
    void on_buttonBox_accepted();
};

#endif // ONLYVIEW_H
