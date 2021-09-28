#include "institute.h"
//Конструктор по-умолчанию
Institute::Institute() {
  //Присвоение новому объекту нулевого id и пустого имени
  id = 0;
  name = "";
}
//Методы set и get
void Institute::setid(int id) { this->id = id; }
int Institute::getid() { return id; }
void Institute::setname(QString name) {
  if (name.length() < 100)
    this->name = name;
}
QString Institute::getname() { return name; }

//Метод, преобразующий результат
// SQL-запроса в объект института
Institute Institute::fromQuery(QSqlQuery query) {
  id = query.value(0).toInt(); //Присвоение первого значения запроса id
  name = query.value(1).toString(); //Присвоение второго значения запроса имени
  return *this; //Возращение изменённого объекта
}
