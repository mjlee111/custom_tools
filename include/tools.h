#ifndef TOOLS_H
#define TOOLS_H

#include <QMainWindow>
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

QT_BEGIN_NAMESPACE
namespace Ui
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

public Q_SLOTS:
  void updateFromTimer();
  void getCurrentTimeAsString();
  void on_opacity_valueChanged(int value);

private:
  Ui::tools* ui;

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
};
#endif  // TOOLS_H
