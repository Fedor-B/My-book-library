#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
/* Подключаем заголовочный файл для работы с информацией, которая помещена в базу данных */
#include "database.h"
#include "dialogbook.h"
#include "authorization.h"

#include "checkin.h"
#include "onlyview.h"
#include "privatewindow.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    DataBase        *db;
    QSqlTableModel  *model;
private slots:
    void on_pushButton_clicked();

    void slotCustomMenuRequested(QPoint pos);
    void slotRemoveRecord();
    void slotUpdateModels();
    void slotEditRecord();
    void on_outAccButton_clicked();

    void on_tableView_customContextMenuRequested(const QPoint &pos);



    void on_exitButton_clicked();

    void slotViewRecord();
    void slotOpenView();
    void on_pushPrivateButton_clicked();

private:
    Ui::MainWindow  *ui;
    /* В проекте используются объекты для взаимодействия с информацией в базе данных
     * и моделью представления таблицы базы данных
     * */


private:
    /* Также присутствуют два метода, которые формируют модель
     * и внешний вид TableView
     * */
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();
};

#endif // MAINWINDOW_H
