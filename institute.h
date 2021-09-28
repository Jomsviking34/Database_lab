#ifndef INSTITUTE_H
#define INSTITUTE_H
#include <QSqlQuery>
#include <QString>
#include <QVariant>

//Класс, реализующий инстутут
class Institute {
public:
  Institute(); //Конструктор по-умолчанию
  //Методы get и set для закрытых полей
  void setid(int id);
  int getid();
  void setname(QString name);
  QString getname();

  Institute fromQuery(QSqlQuery query); //Метод, преобразующий результат
                                        // SQL-запроса в объект института

  //Поля, недоступные для прямого изменения: id и имя
private:
  unsigned int id;
  QString name;
};

#endif // INSTITUTE_H
