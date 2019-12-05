#include "guestclass.h"
#include "ui_guestclass.h"

GuestClass::GuestClass(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GuestClass)
{
    ui->setupUi(this);

    MainWindow m;


    db = m.db;
    db->connectToDataBase();
    this->setupModel(TABLE,
                     QStringList() << trUtf8("id")
                                   << trUtf8("Название книги")
                                   << trUtf8("Автор")
                                   << trUtf8("Содержание")
                     );

    this->createUI();
}

GuestClass::~GuestClass()
{
    delete ui;
}

void GuestClass::on_exitButton_clicked()
{
    Authorization *auth = new Authorization();
    auth->setWindowTitle(trUtf8("Вход"));
    auth->show();
    this->close();
}

void GuestClass::setupModel(const QString &tableName, const QStringList &headers)
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


void GuestClass::createUI()
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
    model->select(); // Делаем выборку данных из таблицы
}

void GuestClass::slotOpenView()
{
    OnlyView *view = new OnlyView(ui->tableView->selectionModel()->currentIndex().row());

    /* Выполняем запуск диалогового окна
     * */
    view->setWindowTitle(trUtf8("Просмотр книги"));
    view->exec();
}

