#ifndef TOOLS_H
#define TOOLS_H

#include <QMainWindow>
#include <QtWidgets>

#include <ctime>
#include <iostream>
#include <QTimer>
#include <QFont>
#include <QDesktopWidget>
#include <QScreen>
#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_series_data.h>
#include <QTimer>
#include <fstream>
#include <string>
#include <sstream>
#include "sys/types.h"
#include "sys/sysinfo.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define WINDOW_WIDTH 190
#define WINDOW_HEIGHT 500
#define MACRO_1_TEXT "Macro 1 is not set !"

QT_BEGIN_NAMESPACE namespace Ui
{
  class tools;
}
QT_END_NAMESPACE

class tools : public QMainWindow
{
  Q_OBJECT

public:
  tools(QWidget* parent = nullptr);
  ~tools();

  std::string time_string;

  static const int num_points = 100;
  int counter = 0;

  QVector<QPointF> cpu_points;
  QVector<QPointF> memory_physical_points;
  QVector<QPointF> memory_virtual_points;

  unsigned long long lastTotalUser, lastTotalUserLow, lastTotalSys, lastTotalIdle;

protected:
public Q_SLOTS:
  void updateFromTimer();
  void getCurrentTimeAsString();
  void on_opacity_valueChanged(int value);
  void on_macro_btn_1_clicked();
  void on_macro_setup_btn_1_clicked();
  void on_move_btn_clicked();

private:
  Ui::tools* ui;
  bool is_movable = false;

  std::string macro_1_name = "macro 1";
  std::string macro_1 = "echo " MACRO_1_TEXT;

  QString macro[3];
  QString macro_name[3];

  QTimer* _500ms_timer;
  QTimer* _10ms_timer;

  QwtPlotCurve* cpu_curve;
  QwtPlotCurve* memory_physical_curve;
  QwtPlotCurve* memory_virtual_curve;

  void uiInitialize();
  void updateCPUPlot();
  double getCPUUsage();
  double getVirtualMemory();
  double getPhysicalMemory();
  void updateMemoryPlot();
  void showMacroSettingWindow(const QString& windowTitle, int macro_number);
  void saveToConfigFile(const QString& macroCmd, const QString& macroName, int macro_number);
  void read_config(QString filename, QString macro[], QString macro_name[]);
};
#endif  // TOOLS_H
