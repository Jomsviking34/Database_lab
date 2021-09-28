#include "mainwindow.h"

#include <QApplication>
#include <QTextCodec>

int main(int argc, char *argv[]) {
  //Установка кодировки UTF-8
  QTextCodec *utfcodec = QTextCodec::codecForName("UTF-8");
  QTextCodec::setCodecForLocale(utfcodec);

  QApplication a(argc, argv);
  MainWindow w;
  w.setWindowTitle("Database");
  w.show();
  return a.exec();
}
