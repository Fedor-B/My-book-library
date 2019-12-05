#include "databaseprivate.h"

DataBasePrivate::DataBasePrivate(QObject *parent) : QObject(parent)
{

}

DataBasePrivate::~DataBasePrivate()
{

}

/* Методы для подключения к базе данных
 * */
void DataBasePrivate::connectToDataBasePrivate()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    if(!QFile("D:/WorkSpace/5sem/QTProjects/projectOne/projectOne/" DATABASEPRIVATE_NAME).exists()){
        this->restoreDataBasePrivate();
    } else {
        this->openDataBasePrivate();
    }
}

/* Методы восстановления базы данных
 * */
bool DataBasePrivate::restoreDataBasePrivate()
{
    // Если база данных открылась ...
    if(this->openDataBasePrivate()){
        // Производим восстановление базы данных
        return (this->createTablePrivate()) ? true : false;
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }

}

/* Метод для открытия базы данных
 * */
bool DataBasePrivate::openDataBasePrivate()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    dbPrivate = QSqlDatabase::addDatabase("QSQLITE");
    dbPrivate.setHostName(DATABASEPRIVATE_HOSTNAME);
    dbPrivate.setDatabaseName("D:/WorkSpace/5sem/QTProjects/projectOne/projectOne/" DATABASEPRIVATE_NAME);
    if(dbPrivate.open()){
        return true;
    } else {
        return false;
    }
}

/* Методы закрытия базы данных
 * */
void DataBasePrivate::closeDataBasePrivate()
{
    dbPrivate.close();
}

/* Метод для создания таблицы в базе данных
 * */
bool DataBasePrivate::createTablePrivate()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением.
     * */
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " TABLEPRIVATE " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            TABLEPRIVATE_BOOKNAME     " VARCHAR(255)    NOT NULL,"
                            TABLEPRIVATE_AUTHOR     " VARCHAR(255)    NOT NULL,"
                            TABLEPRIVATE_TEXT       " VARCHAR(255)    NOT NULL"
                        " )"
                    )){
        qDebug() << "DataBase: error of create " << TABLEPRIVATE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }

}

/* Метод для вставки записи в базу данных
 * */
bool DataBasePrivate::inserIntoTablePrivate(const QVariantList &data)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery queryPrivate;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    queryPrivate.prepare("INSERT INTO " TABLEPRIVATE " ( " TABLEPRIVATE_BOOKNAME ", "
                                             TABLEPRIVATE_AUTHOR ", "
                                             TABLEPRIVATE_TEXT " ) "
                  "VALUES (:BookNamePrivate, :AuthorPrivate, :TextPrivate)");
    queryPrivate.bindValue(":BookNamePrivate",       data[0].toString());
    queryPrivate.bindValue(":AuthorPrivate",       data[1].toString());
    queryPrivate.bindValue(":TextPrivate",         data[2].toString());

    // После чего выполняется запросом методом exec()
    if(!queryPrivate.exec()){
        qDebug() << "error insert into " << TABLEPRIVATE;
        qDebug() << queryPrivate.lastError().text();
        return false;
    } else {
        return true;
    }

}

/* Второй метод для вставки записи в базу данных
 * */
bool DataBasePrivate::inserIntoTablePrivate(const QString &nameBook, const QString &author, const QString &textBook)
{
    QVariantList data;
    data.append(nameBook);
    data.append(author);
    data.append(textBook);

    if(inserIntoTablePrivate(data))
        return true;
    else
        return false;
}

/* Метод для удаления записи из таблицы
 * */
bool DataBasePrivate::removeRecordPrivate(const int id)
{
    // Удаление строки из базы данных будет производитсья с помощью SQL-запроса
    QSqlQuery query;

    // Удаление производим по id записи, который передается в качестве аргумента функции
    query.prepare("DELETE FROM " TABLEPRIVATE " WHERE id= :ID ;");
    query.bindValue(":ID", id);

    // Выполняем удаление
    if(!query.exec()){
        qDebug() << "error delete row " << TABLEPRIVATE;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }

}
