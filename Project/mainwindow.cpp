#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    /* Первым делом необходимо создать объект, который будет использоваться для работы с данными нашей БД
     * и инициализировать подключение к базе данных
     * */
    db = new DataBase();
    db->connectToDataBase();


    /* Инициализируем модель для представления данных
     * с заданием названий колонок
     * */
    this->setupModel(TABLE,
                     QStringList() << trUtf8("id")
                                   << trUtf8("Название книги")
                                   << trUtf8("Автор")
                                   << trUtf8("Содержание")
                     );

    /* Инициализируем внешний вид таблицы с данными
     * */
    this->createUI();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/* Метод для инициализации модеи представления данных
 * */
void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * с установкой имени таблицы в базе данных, по которому
     * будет производится обращение в таблице
     * */
    model = new QSqlTableModel(this);
    model->setTable(tableName);
    model->select();
    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
    for(int i = 0, j = 0; i < model->columnCount(); i++, j++){
        model->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
    // Устанавливаем сортировку по возрастанию данных по нулевой колонке
    model->setSort(0,Qt::AscendingOrder);
    model->select();
}

void MainWindow::createUI()
{
    ui->tableView->setModel(model);     // Устанавливаем модель на TableView
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
    model->select(); // Делаем выборку данных из таблицы
}



void MainWindow::on_pushButton_clicked()
{
    if(ui->bookNameLineEdit->text()!=nullptr&&ui->authorLineEdit->text()!=nullptr&&ui->bookTextLineEdit->text()!=nullptr)
    {
        DataBase database;

        database.inserIntoTable(ui->bookNameLineEdit->text(),ui->authorLineEdit->text(),ui->bookTextLineEdit->text());
        model->insertRow(model->rowCount(QModelIndex()));
        model->submitAll();
        model->select();
    }
    else
    {
        QMessageBox::warning(this,"Внимание!","Данные введены не полностью!");
    }
}

void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    /* Создаем объект контекстного меню */
    QMenu * menu = new QMenu(this);
    /* Создаём действия для контекстного меню */
    QAction * editBook = new QAction(trUtf8("Редактировать"), this);
    QAction * deleteBook = new QAction(trUtf8("Удалить"), this);
    QAction * viewBook = new QAction(trUtf8("Просмотреть"),this);
    /* Подключаем СЛОТы обработчики для действий контекстного меню */
    connect(editBook, SIGNAL(triggered()), this, SLOT(slotEditRecord()));     // Обработчик вызова диалога редактирования
    connect(deleteBook, SIGNAL(triggered()), this, SLOT(slotRemoveRecord())); // Обработчик удаления записи
    connect(viewBook, SIGNAL(triggered()), this, SLOT(slotViewRecord()));
    /* Устанавливаем действия в меню */
    menu->addAction(editBook);
    menu->addAction(deleteBook);
    menu->addAction(viewBook);
    /* Вызываем контекстное меню */
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}


void MainWindow::slotRemoveRecord()
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
            if(!model->removeRow(row)){
                QMessageBox::warning(this,trUtf8("Уведомление"),
                                     trUtf8("Не удалось удалить книгу\n"
                                            "Возможно она используется другими пользователями\n"
                                            "Проверьте все зависимости и повторите попытку"));
            }
            model->select();
            ui->tableView->setCurrentIndex(model->index(-1, -1));
        }
    }
}

/* Слот обновления модели представления данных
 * */
void MainWindow::slotUpdateModels()
{
    model->select();
    ui->tableView->resizeColumnsToContents();
}

/* Метод для активации диалога добавления записей в режиме редактирования
 * с передачей индекса выбранной строки
 * */
void MainWindow::slotEditRecord()
{
    /* Также создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных, но передаём
     * в качестве параметров строку записи
     * */
    DialogBook *bookDialog = new DialogBook(ui->tableView->selectionModel()->currentIndex().row());
    connect(bookDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    bookDialog->setWindowTitle(trUtf8("Редактировать Устройство"));
    bookDialog->exec();
}





void MainWindow::on_outAccButton_clicked()
{
    if (QMessageBox::warning(this,
                             trUtf8("Внимание!"),
                             trUtf8("Вы действительно хотите удалить учетную запись?"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {

        CheckIn check;
        check.loginRemove();
        check.passwordRemove();
        Authorization *w = new Authorization();
        w->setWindowTitle(trUtf8("Вход"));
        w->show();
        this->close();
    }





}

void MainWindow::slotViewRecord()
{
    OnlyView *view = new OnlyView(ui->tableView->selectionModel()->currentIndex().row());

    /* Выполняем запуск диалогового окна
     * */
    view->setWindowTitle(trUtf8("Просмотр книги"));
    view->exec();
}

void MainWindow::on_exitButton_clicked()
{
    Authorization *auth = new Authorization();
    auth->setWindowTitle(trUtf8("Вход"));
    auth->show();
    this->close();
}

void MainWindow::slotOpenView()
{
    OnlyView *view = new OnlyView(ui->tableView->selectionModel()->currentIndex().row());

    /* Выполняем запуск диалогового окна
     * */
    view->setWindowTitle(trUtf8("Просмотр книги"));
    view->exec();
}

void MainWindow::on_pushPrivateButton_clicked()
{
    PrivateWindow *window = new PrivateWindow();
    window->setWindowTitle(trUtf8("Приватная библиотека"));
    window->show();
    this->close();
}
