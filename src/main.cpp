#include "./include/tools.h"

#include <QApplication>

int main(int argc, char* argv[])
{
  QApplication a(argc, argv);
  tools w;
  w.show();
  return a.exec();
}
