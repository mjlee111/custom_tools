#include "./include/tools.h"
#include "./ui_tools.h"

tools::tools(QWidget* parent) : QMainWindow(parent), ui(new Ui::tools)
{
  ui->setupUi(this);

  cpu_curve = new QwtPlotCurve("CPU");
  memory_physical_curve = new QwtPlotCurve("CPU");

  cpu_points.resize(num_points);
  memory_physical_points.resize(num_points);

  uiInitialize();

  _500ms_timer = new QTimer(this);
  _10ms_timer = new QTimer(this);

  connect(_500ms_timer, SIGNAL(timeout()), this, SLOT(updateFromTimer()));
  connect(_10ms_timer, SIGNAL(timeout()), this, SLOT(getCurrentTimeAsString()));

  _500ms_timer->start(500);
  _10ms_timer->start(10);
}

tools::~tools()
{
  delete ui;
}

void tools::uiInitialize()
{
  QScreen* screen = QGuiApplication::primaryScreen();
  QRect mainScreenGeometry = screen->geometry();
  int desktopWidth = mainScreenGeometry.width();
  int desktopHeight = mainScreenGeometry.height();
  std::cout << desktopWidth << " " << desktopHeight << std::endl;
  this->setGeometry(desktopWidth - WINDOW_WIDTH, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
  this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
  this->setWindowOpacity(0.8);
  this->setWindowFlags(Qt::Window | Qt::FramelessWindowHint | Qt::Tool);
  ui->cpu->setAxisScale(QwtPlot::yLeft, 0.0, 100.0);
  ui->cpu->enableAxis(QwtPlot::xBottom, false);
  ui->cpu->enableAxis(QwtPlot::yLeft, false);
  ui->memory->setAxisScale(QwtPlot::yLeft, 0.0, 100.0);
  ui->memory->enableAxis(QwtPlot::xBottom, false);
  ui->memory->enableAxis(QwtPlot::yLeft, false);
  QPen greenPen(Qt::green);
  QPen yellowPen(Qt::yellow);

  cpu_curve->setPen(greenPen);
  memory_physical_curve->setPen(yellowPen);
  FILE* file = fopen("/proc/stat", "r");
  fscanf(file, "cpu %llu %llu %llu %llu", &lastTotalUser, &lastTotalUserLow, &lastTotalSys, &lastTotalIdle);
  fclose(file);
}

void tools::updateFromTimer()
{
  getCurrentTimeAsString();
  updateCPUPlot();
  updateMemoryPlot();
}

void tools::getCurrentTimeAsString()
{
  time_t now = time(nullptr);
  struct tm timeinfo;
  localtime_r(&now, &timeinfo);
  char buffer[80];
  strftime(buffer, sizeof(buffer), "%H:%M:%S", &timeinfo);

  time_string = std::string(buffer);
  ui->time->setStyleSheet("font-size: 24pt;");
  ui->time->setText(QString::fromStdString(time_string));
}

void tools::updateCPUPlot()
{
  double cpu_usage = getCPUUsage();
  QString cpu_usage_string = QString::number(cpu_usage, 'f', 2) + "%";
  ui->cpu_percent->setText(cpu_usage_string);
  QPointF newPoint(counter, cpu_usage);
  cpu_points.append(newPoint);
  if (cpu_points.size() >= num_points)
  {
    cpu_points.removeFirst();
  }
  ui->cpu->setAxisScale(QwtPlot::yLeft, 0.0, 100.0);
  QwtPointSeriesData* seriesData = new QwtPointSeriesData(cpu_points);
  cpu_curve->setSamples(seriesData);
  cpu_curve->attach(ui->cpu);
  ui->cpu->replot();

  counter++;
}

double tools::getCPUUsage()
{
  double percent;
  FILE* file;
  unsigned long long totalUser, totalUserLow, totalSys, totalIdle, total;

  file = fopen("/proc/stat", "r");
  fscanf(file, "cpu %llu %llu %llu %llu", &totalUser, &totalUserLow, &totalSys, &totalIdle);
  fclose(file);

  if (totalUser < lastTotalUser || totalUserLow < lastTotalUserLow || totalSys < lastTotalSys ||
      totalIdle < lastTotalIdle)
  {
    //오버플로우 detection
    percent = -1.0;
  }
  else
  {
    total = (totalUser - lastTotalUser) + (totalUserLow - lastTotalUserLow) + (totalSys - lastTotalSys);
    percent = total;
    total += (totalIdle - lastTotalIdle);
    percent /= total;
    percent *= 100;
  }

  lastTotalUser = totalUser;
  lastTotalUserLow = totalUserLow;
  lastTotalSys = totalSys;
  lastTotalIdle = totalIdle;

  return percent;
}

double tools::getVirtualMemory()
{
  struct sysinfo memInfo;

  sysinfo(&memInfo);
  double virtualMemUsed = memInfo.totalram - memInfo.freeram;
  virtualMemUsed += memInfo.totalswap - memInfo.freeswap;
  virtualMemUsed *= memInfo.mem_unit;
  return virtualMemUsed;
}

double tools::getPhysicalMemory()
{
  struct sysinfo memInfo;
  sysinfo(&memInfo);

  double totalMemory = memInfo.totalram;
  double freeMemory = memInfo.freeram;

  double usedMemory = totalMemory - freeMemory;
  double memoryUsagePercent = (usedMemory / totalMemory) * 100.0;

  return memoryUsagePercent;
}

void tools::updateMemoryPlot()
{
  double memory_usage = getPhysicalMemory();
  QString memory_usage_string = QString::number(memory_usage, 'f', 2) + "%";
  ui->memory_percent->setText(memory_usage_string);
  QPointF newMemoryPoint(counter, memory_usage);
  memory_physical_points.append(newMemoryPoint);
  if (memory_physical_points.size() >= num_points)
  {
    memory_physical_points.removeFirst();
  }
  ui->memory->setAxisScale(QwtPlot::yLeft, 0.0, 100.0);
  QwtPointSeriesData* memorySeriesData = new QwtPointSeriesData(memory_physical_points);
  memory_physical_curve->setSamples(memorySeriesData);
  memory_physical_curve->attach(ui->memory);
  ui->memory->replot();
}

void tools::on_opacity_valueChanged(int value)
{
  double opacity = (double)value / 100;
  this->setWindowOpacity(opacity);
}