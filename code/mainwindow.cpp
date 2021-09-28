#include "mainwindow.h"
#include "ui_mainwindow.h"

QVector<Institute> Institutes; //Объявление списка институтов
QVector<Task> Tasks;           //Объявление списка задач
QSqlDatabase *db; //Объявление указателя на канал связи с бд
Query *Query_1; //Объявление указателя на объект запроса

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->Objects->setToolTip("Выбор");
  ui->Save->setDisabled(true);
  ui->Delete_1->setDisabled(true);
  //Блокировка кнопок взаимодействия с бд
  ui->Load->setDisabled(true);
  ui->Load_2->setDisabled(true);
  ui->Queries->setDisabled(true);
  ui->Query->setDisabled(true);
  ui->Update->setDisabled(true);
  //Соединение объектов интерфейса со слотами
  connect(ui->Objects, SIGNAL(currentIndexChanged(int)), this,
          SLOT(Changeover()));
  connect(ui->Connect, SIGNAL(clicked()), this, SLOT(Connect()));
  connect(ui->Create, SIGNAL(clicked()), this, SLOT(Create()));
  connect(ui->Save, SIGNAL(clicked()), this, SLOT(Save()));
  connect(ui->Institutes, SIGNAL(itemClicked(QListWidgetItem *)), this,
          SLOT(Select()));
  connect(ui->Tasks, SIGNAL(itemClicked(QListWidgetItem *)), this,
          SLOT(Select()));
  connect(ui->Load, SIGNAL(clicked()), this, SLOT(Load()));
  connect(ui->Delete_1, SIGNAL(clicked()), this, SLOT(Delete_1()));
  connect(ui->Load_2, SIGNAL(clicked()), this, SLOT(SaveToDB()));
  connect(ui->Update, SIGNAL(clicked()), this, SLOT(Update()));
  connect(ui->Query, SIGNAL(clicked()), this, SLOT(Query_exec()));
  Changeover();
  ClearWidgets();
  Refresh();
}

//Метод, вызываемый при закрытии приложения
MainWindow::~MainWindow() {
  //Освобождение памяти, выделенной под объекты
  delete db;
  delete Query_1;
  delete ui;
}

//Слот очистки виджетов интерфейса
void MainWindow::ClearWidgets() {
  ui->Name->setText("");
  ui->Status->setText("");
  ui->Subject->setText("");
  ui->Query_line->setText("");
  ui->Statement_date->setDate(QDate(2000, 1, 1));
  ui->Solution_date->setDate(QDate(2000, 1, 1));
}

//Функция обновления состояния виджетов
void MainWindow::Refresh() {
  //Если в таблицах есть записи-разблокировать кнопки сохранения и удаления
  if (ui->Institutes->count() > 0 || ui->Tasks->count() > 0) {
    ui->Save->setEnabled(true);
    ui->Delete_1->setEnabled(true);
    //Если соединение с бд установлено-разблокировать виджеты для взаимодействия
    //с бд
    if (ui->Load->isEnabled()) {
      ui->Load_2->setEnabled(true);
      ui->Query->setEnabled(true);
      ui->Queries->setEnabled(true);
      ui->Update->setEnabled(true);
    }
  } //Если записей нет-заблокировать все виджеты, кроме кнопки добаления записи
  else {
    ui->Save->setDisabled(true);
    ui->Delete_1->setDisabled(true);
    ui->Load_2->setDisabled(true);
    ui->Query->setDisabled(true);
    ui->Queries->setDisabled(true);
    ui->Update->setDisabled(true);
  }
}

//Слот переключения между типами объектов
void MainWindow::Changeover() {
  ClearWidgets(); //Очистка виджетов
  switch (ui->Objects->currentIndex()) {
  //Выбран слот институтов-блокировка виджетов, предназначенных для ввода данных
  //о задачах
  case 0: {
    ui->Status->setDisabled(true);
    ui->Status->setDisabled(true);
    ui->Statement_date->setDisabled(true);
    ui->Solution_date->setDisabled(true);
    ui->Institute_name->setDisabled(true);
    ui->Name->setEnabled(true);
    break;
  }
    //Выбран слот задачблокировка виджетов, предназначенных для ввода данных
    //о институтах
  case 1: {
    ui->Status->setEnabled(true);
    ui->Status->setEnabled(true);
    ui->Statement_date->setEnabled(true);
    ui->Solution_date->setEnabled(true);
    ui->Institute_name->setEnabled(true);
    ui->Name->setDisabled(true);
    break;
  }
  }
}

//Слот создания записи
void MainWindow::Create() {
  ClearWidgets(); //Очистка виджетов
  switch (ui->Objects->currentIndex()) {
  //Выбран слот институтов-создание нового объекта в списке и в виджете и
  //перенос фокуса на него
  case 0: {
    Institutes.append(Institute());
    ui->Institutes->insertItem(ui->Institutes->count(), "Институт");
    ui->Institutes->setCurrentRow(ui->Institutes->count() - 1);
    ui->Institute_name->insertItem(ui->Institute_name->count(), "Институт");
    break;
  }
    //Выбран слот задач-создание нового объекта в списке и в виджете и
    //перенос фокуса на него
  case 1: {
    Tasks.append(Task());
    ui->Tasks->insertItem(ui->Tasks->count(), "Задача");
    ui->Tasks->setCurrentRow(ui->Tasks->count() - 1);
    break;
  }
  }
  Refresh();
}

//Слот сохранения записи
void MainWindow::Save() {
  switch (ui->Objects->currentIndex()) {
  //Выбран слот институтов-сохранение введённых данных в список и обновление
  //записи в таблице
  case 0: {
    Institutes[ui->Institutes->currentRow()].setname(
        ui->Name->text()); //Сохранение введённого имени
    QListWidgetItem *item = ui->Institutes->item(
        ui->Institutes->currentRow()); //Инициализация указателя на изменяемую
                                       //запись в таблице
    item->setText(Institutes[ui->Institutes->currentRow()]
                      .getname()); //Изменение текста в записи
    ui->Institute_name->setItemText(
        ui->Institutes->currentRow(),
        ui->Name->text()); //Изменение текста в списке институтов
    break;
  } //Выбран слот задач-сохранение введённых данных в список и обновление
    //записи в таблице
  case 1: {
    Tasks[ui->Tasks->currentRow()].setstatus(
        ui->Status->text()); //Сохранение введённого статуса
    Tasks[ui->Tasks->currentRow()].setsubject(
        ui->Subject->text()); //Сохранение введённого предмета исследования
    Tasks[ui->Tasks->currentRow()].setinstitutefk(
        ui->Institute_name
            ->currentText()); //Сохранение выбранного имени института
    Tasks[ui->Tasks->currentRow()].setinstituteid(
        Institutes[ui->Institute_name->currentIndex()]
            .getid()); //Сохранение выбранного id института
    Tasks[ui->Tasks->currentRow()].setstatementdate(
        ui->Statement_date
            ->date()); //Сохранение введённой даты постановки задачи
    //Проверка флажка завершённости исследования
    if (ui->isNull->isChecked())
      //Если флажок установлен-присвоение null дате окончания исследования
      Tasks[ui->Tasks->currentRow()].setsolutiondate(QDate(NULL, NULL, NULL));
    else
      // Если флажок не установлен-сохранение выбранной даты окончания
      Tasks[ui->Tasks->currentRow()].setsolutiondate(ui->Solution_date->date());
    QListWidgetItem *item = ui->Tasks->item(
        ui->Tasks->currentRow()); //Инициализация указателя на изменяемую
    //запись в таблице
    item->setText((Tasks[ui->Tasks->currentRow()].getsubject())
                      .append("\t")
                      .append(Tasks[ui->Tasks->currentRow()]
                                  .getstatus())); // Изменение текста в записи
    break;
  }
  }
}

//Слот выбора записи в таблице
void MainWindow::Select() {
  //Если выбрана запись в таблице институтов-переключение в режим работы с
  //институтами
  if (ui->Institutes->hasFocus()) {
    ui->Objects->setCurrentIndex(0);
  }
  //Если выбрана запись в таблице задач-переключение в режим работы с
  //задачами
  if (ui->Tasks->hasFocus()) {
    ui->Objects->setCurrentIndex(1);
  }
  ClearWidgets(); //Очистка виджетов
  switch (ui->Objects->currentIndex()) {
  case 0: {
    ui->Name->setText(
        Institutes[ui->Institutes->currentRow()]
            .getname()); //Запись в виджет сохранённого имени института
    break;
  }
  case 1: {
    //Запись в виджеты сохранённых полей объекта задачи
    ui->Subject->setText(Tasks[ui->Tasks->currentRow()].getsubject());
    ui->Status->setText(Tasks[ui->Tasks->currentRow()].getstatus());
    ui->Statement_date->setDate(
        Tasks[ui->Tasks->currentRow()].getstatementdate());
    //Если дата окончания записана в поле объекта-записать её в виджет
    if (!Tasks[ui->Tasks->currentRow()].getsolutiondate().isNull()) {
      ui->isNull->setChecked(false);
      ui->Solution_date->setDate(
          Tasks[ui->Tasks->currentRow()].getsolutiondate());
    }
    //Если даты окончания нет-установка флажка
    else {
      ui->isNull->setChecked(true);
    }
    //Подсветка связанной с задачей записи института
    ui->Name->setText(Tasks[ui->Tasks->currentRow()].getinstitutefk());
    if (Tasks[ui->Tasks->currentRow()].getinstitutefk().length() > 0) {
      int r = 0;
      int id = 0;
      while (id != Tasks[ui->Tasks->currentRow()].getinstituteid()) {
        id = Institutes[r].getid();
        r++;
      }
      ui->Institutes->setCurrentRow(--r);
      ui->Institute_name->setCurrentIndex(r);
    }
    break;
  }
  }
}

//Слот удаления записи
void MainWindow::Delete_1() {
  switch (ui->Objects->currentIndex()) {
  //Если запись одна-очистить все таблицы
  case 0: {
    if (ui->Institutes->count() == 1) {
      ui->Institutes->clear();
      ui->Institute_name->clear();
      Institutes.clear();
    }
    //Если записей в списке больше одной
    if (ui->Institutes->count() > 1) {
      //Если связь с бд установлена-запросить кол-во связанных с институтом
      //задач
      if (db != nullptr) {
        //Если связанные задачи существуют-вывести предупреждение пользователю
        //об удалении связанных записей
        if (Query_1->Tasks_count(Institutes[ui->Institutes->currentRow()])
                .toInt() > 0) {
          QMessageBox msb(
              QMessageBox::Warning, "Предупреждение",
              "Удаление этого объекта приведёт к удалению объектов, "
              "связанных с ним, продолжить?",
              0, this);
          msb.addButton("Да", QMessageBox::AcceptRole);
          msb.addButton("Нет", QMessageBox::RejectRole);
          //Если ответ положителен
          if (msb.exec() == QMessageBox::AcceptRole) {
            int institute_id_fk =
                Institutes[ui->Institutes->currentRow()]
                    .getid(); //Сохранить в переменной id института
            Institutes.remove(
                ui->Institutes->currentRow()); //Удалить запись в списке
            //Удалить все связанные записи задач-перебор всех записей в списке
            //задач
            for (int i = 0; i < Tasks.count(); i++) {
              //Если сохранённый id совпадает с id сохранённом в записи задачи
              if (Tasks[i].getinstituteid() == institute_id_fk) {
                Tasks.remove(i); //Удалить связанную запись
                QListWidgetItem *item = ui->Tasks->takeItem(
                    i); //Получить указатель на запись из таблицы задач
                ui->Tasks->removeItemWidget(item); //Удалить запись из таблицы
                delete item; //Удалить указатель на запись
                --i; //Перейти к предыдущей записи
              }
              QListWidgetItem *item = ui->Institutes->takeItem(
                  ui->Institutes->currentRow()); //Получить указатель на запись
                                                 //из институтов
              ui->Institute_name->removeItem(
                  ui->Institutes->currentRow()); //Удалить запись из таблицы
              ui->Institutes->removeItemWidget(
                  item);   //Удалить запись из таблицы
              delete item; //Удалить указатель на запись
            }
          }
        } //Если связанных задач нет-удалить запись института
        else {
          Institutes.remove(ui->Institutes->currentRow());
          QListWidgetItem *item =
              ui->Institutes->takeItem(ui->Institutes->currentRow());
          ui->Institute_name->removeItem(ui->Institutes->currentRow());
          ui->Institutes->removeItemWidget(item);
          delete item;
        }
      } //Если связанных задач нет-удалить запись института
      else {
        Institutes.remove(ui->Institutes->currentRow());
        QListWidgetItem *item =
            ui->Institutes->takeItem(ui->Institutes->currentRow());
        ui->Institute_name->removeItem(ui->Institutes->currentRow());
        ui->Institutes->removeItemWidget(item);
        delete item;
      }
    }
    break;
  }
  case 1: {
    if (ui->Tasks->count() == 1) {
      ui->Tasks->clear();
      Tasks.clear();
    }
    if (ui->Tasks->count() > 1) {
      QListWidgetItem *item = ui->Tasks->takeItem(ui->Tasks->currentRow());
      ui->Tasks->removeItemWidget(item);
      delete item;
      Tasks.remove(ui->Tasks->currentRow());
    }
    break;
  }
  }
  Refresh();
}

//Слот соединения с бд
void MainWindow::Connect() {
  db = new QSqlDatabase(QSqlDatabase::addDatabase(
      "QMYSQL")); //Выделение памяти под объект связи с бд
  QFile JsonFile("SQL.json"); //Создать объект файла связанный с файлом JSON,
                              //хранящим данные для подключения к бд
  JsonFile.open(QIODevice::ReadOnly | QIODevice::Text); //Открыть файл
  QString JString = QString::fromLocal8Bit(
      JsonFile.readAll()); //Считать содержимое файла в виде строки
  JsonFile.close(); //Закрыть файл
  QJsonDocument document = QJsonDocument::fromJson(
      JString.toUtf8()); //Создать объект JSON документа из сохранённой строки
  QJsonObject object =
      document.object(); //Создать JSON объект из JSON документа
  db->setPort(object["Port"].toInt()); //Установить порт соединения с бд
  db->setHostName(
      object["HostName"].toString()); //Установить имя хоста соединения с бд
  db->setDatabaseName(object["DatabaseName"].toString()); //Установить имя бд
  db->setUserName(
      object["UserName"]
          .toString()); //Установить имя пользователя для соединения с бд
  db->setPassword(
      object["Password"].toString()); //Установить пароль для соединения с бд
  QMessageBox msg; //Создать объект сообщения
  //Если соединение с бд успешно установлено
  if (db->open()) {
    msg.setText("Успешное подключение"); //Установить текст в сообщении
    Query_1 = new Query; //Выделить память для объекта запроса
    ui->Load->setEnabled(true); //Разблокировать кнопку загрузки
  }
  //Если соединение не установлено
  else {
    msg.setText(
        db->lastError().text()); //Установить текст в сообщении-ошибку бд
  }
  msg.exec(); //Вывести сообщение
}

//Слот загрузки записей из бд
void MainWindow::Load() {
  ui->Objects->setCurrentIndex(0);
  ui->Institutes->clear();
  QVector<Institute> Institute_list = Query_1->Select_Institutes();
  for (int i = 0; i < Institute_list.count(); i++) {
    Create();
    Institutes[i] = Institute_list[i];
    Select();
    Save();
  }
  ui->Objects->setCurrentIndex(1);
  ui->Tasks->clear();
  QVector<Task> Task_list = Query_1->Select_Tasks();
  for (int i = 0; i < Task_list.count(); i++) {
    Create();
    Tasks[i] = Task_list[i];
    Select();
    Save();
  }
  Refresh();
}

//Слот выгрузки записей в бд
void MainWindow::SaveToDB() {
  Query_1->DeleteAll();
  if ((Institutes.count() + Tasks.count()) > 0) {
    Query_1->DeleteAll();
    for (int i = 0; i < Institutes.length(); i++) {
      Query_1->Insert(Institutes[i]);
    }
    for (int i = 0; i < Tasks.length(); i++) {
      Query_1->Insert(Tasks[i]);
    }
  }
  // Load();
}

//Слот обновления записи в бд
void MainWindow::Update() {
  Save(); //Сохранение записи в список
  switch (ui->Objects->currentIndex()) {
  case 0: {
    Query_1->Update(
        Institutes[ui->Institutes->currentRow()]); //Обновление выбранной записи
                                                   //в бд
    break;
  }
  case 1: {
    Query_1->Update(Tasks[ui->Tasks->currentRow()]); //Обновление выбранной
                                                     //записи в бд
    break;
  }
  }
}

//Слот выполнения SQL запроса
void MainWindow::Query_exec() {
  switch (ui->Queries->currentIndex()) {
  case 0: {
    ui->Query_line->setText(Query_1->Tasks_count(
        Institutes[ui->Institutes->currentRow()])); //Запись в виджет разультата
                                                    //запроса кол-ва задач у
                                                    //института
    break;
  }
  case 1: {
    ui->Query_line->setText(Query_1->Success_percent(
        Institutes[ui->Institutes->currentRow()])); //Запись в виджет результата
                                                    //запроса процента успешно
                                                    //выполненных задач
                                                    //института
    break;
  }
  }
}
