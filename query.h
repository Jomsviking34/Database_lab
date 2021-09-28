#ifndef QUERY_H
#define QUERY_H
#include "institute.h"
#include "task.h"
#include <QSqlQuery>
#include <QVariant>

//Класс, реализующий запрос
class Query {
public:
  Query();                 //Конструктор по-умолчанию
  void Update(Task &task); //Метод обновления записи задачи в бд
  void Update(Institute &institute); //Метод обновления записи института в бд
  QString
  Tasks_count(Institute &institute); //Метод, возвращающий результат SQL запроса
  QString Success_percent(
      Institute &institute); //Метод, возвращающий результат SQL запроса
  void DeleteAll(); //Метод, удаляющий все записи в бд
  void DeleteRelated(Institute &institute); //Метод, удаляющий записи, связанные
                                            //с другой записью
  void Insert(Task &task); //Метод, сохраняющий новую запись о задаче в бд
  void Insert(
      Institute &institute); //Метод, сохраняющий новую запись об институте в бд
  QVector<Task> Select_Tasks(); //Метод, возвращающий все записи задач из бд
  QVector<Institute>
  Select_Institutes(); //Метод, возвращающий все записи институтов из бд

  //Поле запроса
private:
  QSqlQuery query;
};

#endif // QUERY_H
