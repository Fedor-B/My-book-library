#include "privatewindow.h"
#include "ui_privatewindow.h"


#include <QMenu>
#include <QMessageBox>
PrivateWindow::PrivateWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrivateWindow)
{
    ui->setupUi(this);


    dbP = new DataBasePrivate();
    dbP->connectToDataBasePrivate();
    this->setupModel(TABLEPRIVATE,
                     QStringList() << trUtf8("id")
                                   << trUtf8("Название книги")
                                   << trUtf8("Автор")
                                   << trUtf8("Содержание")
                     );

    this->createUI();
}

PrivateWindow::~PrivateWindow()
{
    delete ui;
}


void PrivateWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице
     * */
    modelprivate = new QSqlTableModel(this);
    modelprivate->setTable(tableName);
    modelprivate->select();
    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
    for(int i = 0, j = 0; i < modelprivate->columnCount(); i++, j++){
        modelprivate->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    modelprivate->setSort(0,Qt::AscendingOrder);
    modelprivate->select();
}


void PrivateWindow::createUI()
{
    ui->tableView->setModel(modelprivate);     // Устанавливаем модель на TableView
  ui->tableView->setColumnHidden(0, true);    // Скрываем колонку с id записей
    // Разрешаем выделение строк
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    // Устанавливаем режим выделения лишь одно строки в таблице
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    // Устанавливаем размер колонок по содержимому
    ui->tableView->resizeColumnsToContents();
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    connect(ui->tableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotOpenView()));
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));
    modelprivate->select(); // Делаем выборку данных из таблицы
}

void PrivateWindow::slotOpenView()
{
    OnlyViewPrivate *view = new OnlyViewPrivate(ui->tableView->selectionModel()->currentIndex().row());

    /* Выполняем запуск диалогового окна
     * */
    view->setWindowTitle(trUtf8("Просмотр книги"));
    view->exec();
}


void PrivateWindow::on_pushButton_clicked()
{
    if(ui->bookNameLineEdit->text()!=nullptr&&ui->authorLineEdit->text()!=nullptr&&ui->bookTextLineEdit->text()!=nullptr)
    {
        DataBasePrivate databaseprivate;

        databaseprivate.inserIntoTablePrivate(ui->bookNameLineEdit->text(),ui->authorLineEdit->text(),ui->bookTextLineEdit->text());
        modelprivate->insertRow(modelprivate->rowCount(QModelIndex()));
        modelprivate->submitAll();
        modelprivate->select();
    }
    else
    {
        QMessageBox::warning(this,"Внимание!","Данные введены не полностью!");
    }
}
void PrivateWindow::slotCustomMenuRequested(QPoint pos)
{
    /* Создаем объект контекстного меню */
    QMenu * menuprivate = new QMenu(this);
    /* Создаём действия для контекстного меню */
    QAction * editBookp = new QAction(trUtf8("Редактировать"), this);
    QAction * deleteBookp = new QAction(trUtf8("Удалить"), this);
    QAction * viewBookp = new QAction(trUtf8("Просмотреть"),this);
    /* Подключаем СЛОТы обработчики для действий контекстного меню */
    connect(editBookp, SIGNAL(triggered()), this, SLOT(slotEditRecord()));     // Обработчик вызова диалога редактирования
    connect(deleteBookp, SIGNAL(triggered()), this, SLOT(slotRemoveRecord())); // Обработчик удаления записи
    connect(viewBookp, SIGNAL(triggered()), this, SLOT(slotViewRecord()));
    /* Устанавливаем действия в меню */
    menuprivate->addAction(editBookp);
    menuprivate->addAction(deleteBookp);
    menuprivate->addAction(viewBookp);
    /* Вызываем контекстное меню */
    menuprivate->popup(ui->tableView->viewport()->mapToGlobal(pos));
}


void PrivateWindow::slotRemoveRecord()
{
    /* Выясняем, какая из строк была выбрана
     * */
    int row = ui->tableView->selectionModel()->currentIndex().row();
    /* Проверяем, что строка была действительно выбрана
     * */
    if(row >= 0){
        /* Задаём вопрос, стоит ли действительно удалять запись.
         * При положительном ответе удаляем запись
         * */
        if (QMessageBox::warning(this,
                                 trUtf8("Удаление записи"),
                                 trUtf8("Вы уверены, что хотите удалить эту запись?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            /* При отрицательном ответе делаем откат действий
             * и закрываем диалог без удаления записи
             * */
            QSqlDatabase::database().rollback();
            return;
        } else {
            /* В противном случае производим удаление записи.
             * При успешном удалении обновляем таблицу.
             * */
            if(!modelprivate->removeRow(row)){
                QMessageBox::warning(this,trUtf8("Уведомление"),
                                     trUtf8("Не удалось удалить книгу\n"
                                            "Возможно она используется другими пользователями\n"
                                            "Проверьте все зависимости и повторите попытку"));
            }
            modelprivate->select();
            ui->tableView->setCurrentIndex(modelprivate->index(-1, -1));
        }
    }
}

/* Слот обновления модели представления данных
 * */
void PrivateWindow::slotUpdateModels()
{
    modelprivate->select();
    ui->tableView->resizeColumnsToContents();
}

/* Метод для активации диалога добавления записей в режиме редактирования
 * с передачей индекса выбранной строки
 * */
void PrivateWindow::slotEditRecord()
{
    /* Также создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных, но передаём
     * в качестве параметров строку записи
     * */
    DialogBookPrivate *bookDialog = new DialogBookPrivate(ui->tableView->selectionModel()->currentIndex().row());
    connect(bookDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    bookDialog->setWindowTitle(trUtf8("Редактировать Устройство"));
    bookDialog->exec();
}



void PrivateWindow::on_exitButton_clicked()
{
    MainWindow *w = new MainWindow();
    w->setWindowTitle(trUtf8("Библиотека"));
    w->show();
    this->close();
}

void PrivateWindow::slotViewRecord()
{
    OnlyViewPrivate *view = new OnlyViewPrivate(ui->tableView->selectionModel()->currentIndex().row());

    /* Выполняем запуск диалогового окна
     * */
    view->setWindowTitle(trUtf8("Просмотр книги"));
    view->exec();
}



