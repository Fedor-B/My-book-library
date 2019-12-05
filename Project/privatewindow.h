#ifndef PRIVATEWINDOW_H
#define PRIVATEWINDOW_H

#include <QDialog>
#include <QSqlTableModel>
#include "databaseprivate.h"
#include "mainwindow.h"

#include "dialogbookprivate.h"
#include "onlyviewprivate.h"
namespace Ui {
class PrivateWindow;
}

class PrivateWindow : public QDialog
{
    Q_OBJECT

public:
    explicit PrivateWindow(QWidget *parent = 0);
    ~PrivateWindow();
    DataBasePrivate        *dbP;
    QSqlTableModel  *modelprivate;
private slots:
    void on_exitButton_clicked();

    void slotOpenView();
    void on_pushButton_clicked();

    void slotEditRecord();
    void slotUpdateModels();
    void slotRemoveRecord();
    void slotCustomMenuRequested(QPoint pos);
    void slotViewRecord();
private:
    Ui::PrivateWindow *ui;
private:
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();
};

#endif // PRIVATEWINDOW_H
