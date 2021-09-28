#include "task.h"
#include <QVariant>
//Конструктор по-умолчанию
Task::Task() {
  id = 0;
  subject = "";
  statement_date.setDate(2001, 01, 01);
  solution_date.setDate(2001, 01, 01);
  status = "";
  institute_fk = "";
}

//Методы set и get
void Task::setid(int id) { this->id = id; }
int Task::getid() { return id; }
void Task::setinstitutefk(QString institute) { this->institute_fk = institute; }
QString Task::getinstitutefk() { return institute_fk; }
void Task::setstatus(QString status) {
  if (status.length() < 50)
    this->status = status;
}
QString Task::getstatus() { return status; }
void Task::setsubject(QString subject) {
  if (subject.length() < 100)
    this->subject = subject;
}
QString Task::getsubject() { return subject; }
void Task::setstatementdate(QDate statement_date) {
  this->statement_date = statement_date;
}
QDate Task::getstatementdate() { return statement_date; }
void Task::setsolutiondate(QDate solution_date) {
  this->solution_date = solution_date;
}
QDate Task::getsolutiondate() { return solution_date; }
void Task::setinstituteid(int instituteid) { this->institute_id = instituteid; }
int Task::getinstituteid() { return institute_id; }

//Метод, преобразующий результат
// SQL-запроса в объект задачи
Task Task::fromQuery(QSqlQuery query) {
  id = query.value(0).toInt();
  subject = query.value(1).toString();
  statement_date = query.value(2).toDate();
  solution_date = query.value(3).toDate();
  status = query.value(4).toString();
  institute_fk = query.value(5).toString();
  institute_id = query.value(6).toInt();
  return *this;
}
