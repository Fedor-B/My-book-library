#ifndef DATABASEPRIVATE_H
#define DATABASEPRIVATE_H


#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASEPRIVATE_HOSTNAME   "NameDataBasePrivate"
#define DATABASEPRIVATE_NAME       "NamePrivate.db"

#define TABLEPRIVATE                   "NameTablePrivate"         // Название таблицы
#define TABLEPRIVATE_BOOKNAME          "BookNamePrivate"         // Вторая колонка
#define TABLEPRIVATE_AUTHOR             "AuthorPrivate"           // Третья колонка
#define TABLEPRIVATE_TEXT               "TextPrivate"               // Четвертая колонка

// Первая колонка содержит Autoincrement ID

class DataBasePrivate : public QObject
{
    Q_OBJECT
public:
    explicit DataBasePrivate(QObject *parent = 0);
    ~DataBasePrivate();
    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    void connectToDataBasePrivate();

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    dbPrivate;

private:
    /* Внутренние методы для работы с базой данных
     * */
    bool openDataBasePrivate();        // Открытие базы данных
    bool restoreDataBasePrivate();     // Восстановление базы данных
    void closeDataBasePrivate();       // Закрытие базы данных
    bool createTablePrivate();         // Создание базы таблицы в базе данных

public slots:
    bool inserIntoTablePrivate(const QVariantList &data);      // Добавление записей в таблицу
    bool inserIntoTablePrivate(const QString &namebook, const QString &author, const QString &textbook);
    bool removeRecordPrivate(const int id); // Удаление записи из таблицы по её id
};


#endif // DATABASEPRIVATE_H
