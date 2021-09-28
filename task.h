#ifndef TASK_H
#define TASK_H
#include <QDate>
#include <QSqlQuery>
#include <QString>

class Task {
public:
  Task(); //Конструктор по-умолчанию
  //Методы get и set для закрытых полей
  void setid(int id);
  int getid();
  void setinstitutefk(QString institute);
  QString getinstitutefk();
  void setstatus(QString status);
  QString getstatus();
  void setsubject(QString subject);
  QString getsubject();
  void setstatementdate(QDate statement_date);
  QDate getstatementdate();
  void setsolutiondate(QDate solution_date);
  QDate getsolutiondate();
  void setinstituteid(int instituteid);
  int getinstituteid();

  Task fromQuery(QSqlQuery query); //Метод, преобразующий результат
  // SQL-запроса в объект задачи

  //Поля, недоступные для прямого изменения
private:
  unsigned int id;
  QString subject;
  QDate statement_date;
  QDate solution_date;
  QString status;
  QString institute_fk;
  unsigned int institute_id;
};

#endif // TASK_H
