#include "query.h"

Query::Query() {}

//Метод, обновляющий запись задачи в бд
void Query::Update(Task &task) {
  query.prepare("update tasks set subject=:subject, "
                "statement_date=:statement_date,solution_date=:solution_"
                "date ,status=:status ,institute_id_fk "
                "=(select "
                "institute_id from institutes where "
                "institutes.name = :institutefk) where task_id =:id;");
  //Подготовка строки запроса(обновление внешнего ключа-id института происходит
  //через его имя)

  //Подстановка в строку запроса полей объекта задачи через методы
  //класса
  query.bindValue(":subject", task.getsubject());
  query.bindValue(":statement_date", task.getstatementdate());
  query.bindValue(":solution_date", task.getsolutiondate());
  query.bindValue(":status", task.getstatus());
  query.bindValue(":institutefk", task.getinstitutefk());
  query.bindValue(":id", task.getid());

  query.exec();  //Выполнение запроса
  query.clear(); //Очистка запроса
}

//Метод, обновляющий запись института в бд
void Query::Update(Institute &institute) {
  query.prepare("update institutes set name = (:name) where institute_id = "
                "(:id);"); //Подготовка строки запроса
  //Подстановка в строку запроса полей объекта института через методы класса
  query.bindValue(":name", institute.getname());
  query.bindValue(":id", institute.getid());
  query.exec();  //Выполнение запроса
  query.clear(); //Очистка запроса
}

//Метод, возвращающий кол-во задач института в виде строки
QString Query::Tasks_count(Institute &institute) {
  QString Count; //Объявление строки
  query.prepare(
      "select count(subject) as count from institutes inner join tasks on "
      "institute_id_fk=institute_id where institute_id=:id;"); //Подготовка
                                                               //строки запроса
  query.bindValue(
      ":id", institute.getid()); //Подстановка id института в строку запроса
  query.exec(); //Выполнение запроса
  if (query.next())
    Count =
        query.value(0).toString(); //Преобразование полученного ответа в строку
  query.clear();                   //Очистка запроса
  return Count;                    //Возвращение строки
}

//Метод, возвращающий процент успешно выполненных институтом задач в виде строки
QString Query::Success_percent(Institute &institute) {
  QString Percent; //Объявление строки
  query.prepare(
      "select(select count(*) from tasks inner join institutes on "
      "institute_id_fk=institute_id where status='Выполнено'and "
      "institute_id=:id)/(select count(*) from tasks inner join "
      "institutes on "
      "institute_id_fk=institute_id where institute_id=:id);"); //Подготовка
  //строки запроса
  query.bindValue(
      ":id", institute.getid()); //Подстановка id института в строку запроса
  query.exec(); //Выполнение запроса
  if (query.next())
    Percent = QString::number(query.value(0).toDouble() *
                              100); //Умножение, полученного ответа на 100 и его
                                    //преобразование в строку
  query.clear();  //Очистка запроса
  return Percent; //Возвращение строки
}

//Метод, удаляющий все записи в бд
void Query::DeleteAll() {
  query.exec(
      "delete from institutes where institute_id>0;"); //Запрос на удаление
                                                       //записей из таблицы
                                                       //институтов
  query.exec("delete from tasks where task_id>0;"); //Запрос на удаление записей
                                                    //из таблицы задач
  query.exec(
      "alter table institutes auto_increment=0;"); //Запрос на обнуление
                                                   //счётчика автоинкремента
                                                   //таблицы
  query.exec(
      "alter table tasks auto_increment=0;"); //Запрос на обнуление счётчика
                                              //автоинкремента таблицы
  query.clear(); //Очистка запроса
}

//Метод, удаляющий связанные с институтом записи задач
void Query::DeleteRelated(Institute &institute) {
  query.prepare("delete from tasks where institute_id_fk=:id;"); //Подготовка
  //строки запроса
  query.bindValue(
      ":id", institute.getid()); //Подстановка в строку запроса id института
  query.exec();  //Выполнение запроса
  query.clear(); //Очистка запроса
}

//Метод, добавляющий в бд запись задачи
void Query::Insert(Task &task) {
  query.prepare(
      "insert into "
      "tasks(subject,statement_date,solution_date,status,institute_id_fk) "
      "values(:subject,:statement_date,:solution_date,:status,(select "
      "institute_id from institutes where "
      "institutes.name = :institutefk));"); //Подготовка
  //строки запроса
  //Подстановка в строку запроса полей объекта задачи через методы класса
  query.bindValue(":subject", task.getsubject());
  query.bindValue(":statement_date", task.getstatementdate());
  query.bindValue(":solution_date", task.getsolutiondate());
  query.bindValue(":status", task.getstatus());
  query.bindValue(":institutefk", task.getinstitutefk());
  query.exec();                           //Выполнение запроса
  query.exec("select last_insert_id();"); //Запрос на последний добавленный id
  if (query.next())
    task.setid(
        query.value(0).toInt()); //Присвоение объекту задачи полученного id
  query.prepare(
      "select institute_id_fk from tasks where task_id=:id;"); //Подготовка
                                                               //строки запроса
                                                               //на id института
  query.bindValue(":id", task.getid()); //Подстановка id задачи
  query.exec();                         //Выполнение задачи
  if (query.next())
    task.setinstituteid(query.value(0).toInt()); //Присвоение id института
  query.clear();                                 //Очистка запроса
}

//Метод, добавляющий в бд запись института
void Query::Insert(Institute &institute) {
  query.prepare("insert into institutes (name) values(:name)");
  query.bindValue(":name", institute.getname());
  query.exec();                           //Выполнение запроса
  query.exec("select last_insert_id();"); //Запрос на последний добавленный id
  if (query.next())
    institute.setid(
        query.value(0).toInt()); //Присвоение объекту института полученного id
  query.clear();                 //Очистка запроса
}

//Метод, загружающий из бд записи задач
QVector<Task> Query::Select_Tasks() {
  QVector<Task> Tasks; //Объявление списка задач
  query.exec("select "
             "tasks.task_id,subject,statement_date,solution_date,status,"
             "institutes.name,institute_id from tasks inner join institutes "
             "on institute_id_fk=institute_id;"); //Выполнение запроса на
                                                  //получение записей задач
  //Пока существуют не добавленные задачи
  while (query.next()) {
    Tasks.append(Task().fromQuery(
        query)); //Добавить задачу, преобразованную из ответа на запрос в список
  }
  query.clear(); //Очистка запроса
  return Tasks;  //Вернуть список задач
}

//Метод, загружающий из бд записи институтов
QVector<Institute> Query::Select_Institutes() {
  QVector<Institute> Institutes; //Объявление списка институтов
  query.exec("select * from institutes"); //Выполнение запроса на получение
                                          //записей институтов
  //Пока существуют не добавленные институты
  while (query.next()) {
    Institutes.append(
        Institute().fromQuery(query)); //Добавить институт, преобразованный из
                                       //ответа на запрос в список
  }
  query.clear();     //Очистка запроса
  return Institutes; //Вернуть список институтов
}
