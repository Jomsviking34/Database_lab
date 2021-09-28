#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "institute.h"
#include "query.h"
#include "task.h"
#include <QDate>
#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMainWindow>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

private:
  Ui::MainWindow *ui;
  void ClearWidgets();
  void Refresh();
private slots:
  void Changeover();
  void Connect();
  void Create();
  void Save();
  void Select();
  void Load();
  void SaveToDB();
  void Delete_1();
  void Update();
  void Query_exec();
};
#endif // MAINWINDOW_H
