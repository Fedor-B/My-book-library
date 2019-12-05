#include "onlyviewprivate.h"
#include "ui_onlyviewprivate.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlTableModel>
#include <QStandardPaths>

OnlyViewPrivate::OnlyViewPrivate(int row,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OnlyViewPrivate)
{
    ui->setupUi(this);
    setupModel();


    if(row == -1){
        model->insertRow(model->rowCount(QModelIndex()));
    /* В противном случае диалог настраивается на заданную запись
     * */
     mapper->toLast();
    } else {
        mapper->setCurrentModelIndex(model->index(row,0));
    }
}

OnlyViewPrivate::~OnlyViewPrivate()
{
    delete ui;
}
void OnlyViewPrivate::setupModel()
{
    /* Инициализируем модель и делаем выборку из неё
     * */
    model = new QSqlTableModel(this);
    model->setTable(TABLEPRIVATE);
    model->setEditStrategy(QSqlTableModel::OnRowChange);
    model->select();

    mapper = new QDataWidgetMapper();
    mapper->setModel(model);

    ui->nameBookLineEdit->setReadOnly(0);
    ui->authorLineEdit->setReadOnly(0);
    ui->textLineEdit->setReadOnly(0);

    mapper->addMapping(ui->nameBookLineEdit, 1);
    mapper->addMapping(ui->authorLineEdit, 2);
    mapper->addMapping(ui->textLineEdit,3);

    ui->nameBookLineEdit->setReadOnly(1);
    ui->authorLineEdit->setReadOnly(1);
    ui->textLineEdit->setReadOnly(1);

    /* Ручное подтверждение изменения данных
     * через mapper
     * */
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);


    /* Подключаем коннекты от кнопок пролистывания
     * к прилистыванию модели данных в mapper
     * */
    connect(ui->previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    /* При изменении индекса в mapper изменяем состояние кнопок
     * */
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtons(int)));

}



void OnlyViewPrivate::accept()
{
qDebug() << ui->nameBookLineEdit->text();
}

void OnlyViewPrivate::updateButtons(int row)
{
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
}
