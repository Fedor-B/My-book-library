#include "dialogbookprivate.h"
#include "ui_dialogbookprivate.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>

DialogBookPrivate::DialogBookPrivate(int row,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogBookPrivate)
{
    ui->setupUi(this);
    setupModel();

    /* Если строка не задана, то есть равна -1,
     * тогда диалог работает по принципу создания новой записи.
     * А именно, в модель вставляется новая строка и работа ведётся с ней.
     * */
    if(row == -1){
        model->insertRow(model->rowCount(QModelIndex()));
    /* В противном случае диалог настраивается на заданную запись
     * */
     mapper->toLast();
    } else {
        mapper->setCurrentModelIndex(model->index(row,0));
    }
}

DialogBookPrivate::~DialogBookPrivate()
{
    delete ui;
}
void DialogBookPrivate::setupModel()
{
    /* Инициализируем модель и делаем выборку из неё
     * */
    model = new QSqlTableModel(this);
    model->setTable(TABLEPRIVATE);
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();

    mapper = new QDataWidgetMapper();
    mapper->setModel(model);
    mapper->addMapping(ui->bookNameLineEdit, 1);
    mapper->addMapping(ui->authorLineEdit, 2);
    mapper->addMapping(ui->textEdit,3);
    /* Ручное подтверждение изменения данных
     * через mapper
     * */
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    qDebug() << ui->bookNameLineEdit->text();
    /* Подключаем коннекты от кнопок пролистывания
     * к прилистыванию модели данных в mapper
     * */
    connect(ui->previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    /* При изменении индекса в mapper изменяем состояние кнопок
     * */
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtons(int)));

}



void DialogBookPrivate::accept()
{
qDebug() << ui->bookNameLineEdit->text();
}

void DialogBookPrivate::updateButtons(int row)
{
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
}


void DialogBookPrivate::on_buttonBox_accepted()
{
    DataBasePrivate database;
    database.removeRecordPrivate(model->rowCount(QModelIndex()));
    model->removeRow(model->rowCount(QModelIndex()));
    mapper->submit();
    model->submitAll();
    emit signalReady();
    this->close();
}
