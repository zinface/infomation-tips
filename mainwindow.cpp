#include "mainwindow.h"

#include <QApplication>
#include <QDebug>
#include <QDesktopWidget>
#include <QFile>
#include <QLabel>
#include <QTextStream>
#include <QTimer>
#include <QVBoxLayout>
#include <QtMath>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
  ,mCpuUsed(new QLabel())
  ,mMemUsed(new QLabel())
  ,mNetUpload(new QLabel())
  ,mNetDownload(new QLabel())
  ,mUptime(new QLabel())
  ,timer(new QTimer())
  ,old_upload(0)
  ,old_download(0)
{
    getCpuUsed();
    setAttribute(Qt::WA_TransparentForMouseEvents, true);
    setAttribute(Qt::WA_TranslucentBackground, true);

    setWindowFlags(Qt::X11BypassWindowManagerHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
//    setAutoFillBackground(false); //这个不设置的话就背景变黑  -- 无解
//    setWindowOpacity(0);  //0是全透明，1是不透明  窗口及其上面的控件都半透明：
//    setStyleSheet("background-color:transparent;");  //样式表设置透明

//    QPalette pal = palette();
//    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
//    setPalette(pal);  //窗口整体透明，但窗口控件不透明，QLabel控件只是字显示，控件背景色透明； 窗体客户区完全透明

//    QLabel *fankuai = new QLabel("■");

    QVBoxLayout *netLayout = new QVBoxLayout;
    netLayout->addWidget(mNetUpload);
    netLayout->setAlignment(mNetUpload, Qt::AlignRight | Qt::AlignTop);
    netLayout->addWidget(mNetDownload);
    netLayout->setAlignment(mNetDownload, Qt::AlignRight | Qt::AlignTop);

    QVBoxLayout *cpumemLayout = new QVBoxLayout;
    cpumemLayout->addWidget(mCpuUsed);
    cpumemLayout->setAlignment(mCpuUsed, Qt::AlignRight | Qt::AlignTop);
    cpumemLayout->addWidget(mMemUsed);
    cpumemLayout->setAlignment(mMemUsed, Qt::AlignRight | Qt::AlignTop);

    QHBoxLayout *topLayout = new QHBoxLayout;
    topLayout->addLayout(netLayout);
    topLayout->addLayout(cpumemLayout);

    QVBoxLayout *mainLayout = new QVBoxLayout;
//    mainLayout->addWidget(mCpuUsed);
//    mainLayout->setAlignment(mCpuUsed, Qt::AlignRight | Qt::AlignTop);
//    mainLayout->addWidget(mMemUsed);
//    mainLayout->setAlignment(mMemUsed, Qt::AlignRight | Qt::AlignTop);
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(mUptime);
    mainLayout->setAlignment(mUptime, Qt::AlignRight | Qt::AlignTop);
    mainLayout->addStretch();
    QDesktopWidget *localDesktop = QApplication::desktop();
    setFixedSize(200,100);

    setLayout(mainLayout);

    move(localDesktop->width()-this->width(),0);
    timer->setInterval(1000);
    timer->start();

    connect(timer,&QTimer::timeout, this, &MainWindow::onTimerout);
}

long old_cpuAll = 0;
long old_cpuFree = 0;

QString MainWindow::getCpuUsed()
{
//    zinface@zinface-PC:~$ cat /proc/stat  | grep cpu
//    cpu  3507826 1283 1613816 67579694 58751 0 17705 0 0 0
//    cpu0 451290 226 203888 8435772 7428 0 920 0 0 0
//    cpu1 441084 183 203333 8441822 6801 0 1639 0 0 0
//    cpu2 441769 157 203410 8445984 8455 0 1139 0 0 0
//    cpu3 443894 142 202652 8446152 7537 0 1093 0 0 0
//    cpu4 409728 118 200072 8458483 7569 0 5267 0 0 0
//    cpu5 437691 149 197765 8458739 6853 0 900 0 0 0
//    cpu6 439500 134 200032 8447919 7041 0 5902 0 0 0
//    cpu7 442869 172 202661 8444820 7065 0 843 0 0 0
    long cpuAll = 0;
    long cpuFree = 0;
    QFile file("/proc/stat"); // /proc/stat
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    QStringList list = stream.readLine().split(QRegExp("\\s{1,}"));
    foreach (QString str, list){
        cpuAll += str.toLong();
    }
    cpuFree = list.at(4).toLong();

    QString result = QString("CPU:%1%").arg((((cpuAll - old_cpuAll) - (cpuFree - old_cpuFree)) * 100.0 / (cpuAll - old_cpuAll)), 0, 'f', 2, QLatin1Char(' '));
    old_cpuAll = cpuAll;
    old_cpuFree = cpuFree;

    return result;
}

QString MainWindow::getMemUsed()
{
    long memory;
    long memoryAll;
    long swap;
    long swapAll;

    QFile file("/proc/meminfo");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    long buff[16] = {0};
    for (int i = 0; i <= 15; ++i) {
        QString line = stream.readLine();
        QStringList list = line.split(QRegExp("\\s{1,}"));
        buff[i] = list.at(1).toLong();
    }
    memoryAll = buff[0];
    memory = buff[0] - buff[2];
    swapAll = buff[14];
    swap = buff[14] - buff[15];

    QString result = QString("MEM:%1%").arg(memory * 100.0 / memoryAll, 0, 'f', 2, QLatin1Char(' '));

    file.close();

    return result;
}

QString MainWindow::getNetUploadDownload()
{
//    zinface@zinface-PC:~$ cat /proc/net/dev
//    Inter-|   Receive                                                |  Transmit
//     face |bytes    packets errs drop fifo frame compressed multicast|bytes    packets errs drop fifo colls carrier compressed
//        lo: 4112965538 1817828    0    0    0     0          0         0 4112965538 1817828    0    0    0     0       0          0
//    wlp1s0: 5138671304 3912427    0 75719    0     0          0         0 137609139 1533178    0    0    0     0       0          0

    long uploadAll = 0;
    long downloadAll = 0;

    long upload = 0;
    long download = 0;

    QFile file("/proc/net/dev");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    QString line = stream.readLine();
    line  = stream.readLine();
    line  = stream.readLine();
    while (!line.isNull()) {
        line = line.trimmed();
        QStringList list = line.split(QRegExp("\\s{1,}"));   // 匹配任意 大于等于1个的 空白字符

        if (!list.isEmpty()) {
            upload = list.at(9).toLong();
            download = list.at(1).toLong();
        }

        uploadAll += upload;
        downloadAll += download;
        line  = stream.readLine();
    }
    double upspeed = 0;
    SpeedRateUnit upunit = SpeedRateByte;
    QString uploadUnit = autoRateUnits((uploadAll - old_upload) / (timer->interval() / 1000), upunit, upspeed);
    double dospeed = 0;
    SpeedRateUnit dounit = SpeedRateByte;
    QString downUnit = autoRateUnits((downloadAll - old_download) / (timer->interval() / 1000), dounit, dospeed);

//↑:
//↓:
    mNetUpload->setText(QString("↑:%1").arg(upspeed, 0, 'f', 2, QLatin1Char(' ')) + uploadUnit);
    mNetDownload->setText(QString("↓:%1").arg(dospeed, 0, 'f', 2, QLatin1Char(' ')) + downUnit);

    old_upload = uploadAll;
    old_download = downloadAll;

    return QString();
}

//QString MainWindow::getNetDownload()
//{
//    return QString();
//}

QString MainWindow::getUptime()
{
//    zinface@zinface-PC:~$ cat /proc/uptime
//    96557.08 718567.54
    int uptime;
    QFile file("/proc/uptime"); // /proc/uptime
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&file);
    uptime = qFloor(stream.readLine().split(QRegExp("\\s{1,}")).at(0).toDouble());
    int s = 60;
    int m = 60*60;
    int h = 60*60*24;
    int d = 60*60*24;
    int ss =  uptime % s;
    int mm = (uptime % m) / s;
    int hh = (uptime % h) / m;
    int dd = (uptime / d);
    QString runTime = QString("系统已运行: %1天, %2:%3:%4")
            .arg(dd, 0, 'f', 0, QLatin1Char(' '))
            .arg(hh, 2, 'f', 0, QLatin1Char('0'))
            .arg(mm, 2, 'f', 0, QLatin1Char('0'))
            .arg(ss, 2, 'f', 0, QLatin1Char('0'));

    file.close();

    return runTime;
}

QString MainWindow::convertRateUnits(MainWindow::SpeedRateUnit &unit)
{
    switch (unit) {
    case SpeedRateBit:
        return QString("bit/s");
    case SpeedRateByte:
        return QString("b/s");
    case SpeedRateKb:
        return QString("kb/s");
    case SpeedRateMb:
        return QString("mb/s");
    case SpeedRateGb:
        return QString("gb/s");
    case SpeedRateTb:
        return QString("tb/s");
    default:
        return QString("");
    }
}

QString MainWindow::autoRateUnits(long speed, MainWindow::SpeedRateUnit &unit, double &sp)
{
    sp = 0;
    if        (speed >= 0 && speed < qPow(2,10)) { // qPow(2,10) = 1024b
        unit = SpeedRateByte;
        sp = speed;
    } else if (speed >= qPow(2,10) && speed < qPow(2,20)) { // qPow(2,20) = 1024k
        unit = SpeedRateKb;
        sp =  (speed /  qPow(2,10) * 1.0);
    } else if (speed >= qPow(2,20) && speed < qPow(2,30)) { // qPow(2,30) = 1024m
        unit = SpeedRateMb;
        sp =  (speed /  qPow(2,20) * 1.0);
    } else if (speed >= qPow(2,30) && speed < qPow(2,40)) { // qPow(2,40) = 1024g
        unit = SpeedRateGb;
        sp =  (speed /  qPow(2,30) * 1.0);
    } else if (speed >= qPow(2,40) && speed < qPow(2,50)) { // qPow(2,50) = 1024t
        unit = SpeedRateTb;
        sp =  (speed /  qPow(2,40) * 1.0);
    } else {                                                // qPow(2,60) = 1024z => ?
        unit = SpeedRateUnknow;
        sp =  -1;
    }
    speed = sp;
    return convertRateUnits(unit);
}

void MainWindow::onTimerout()
{
    mCpuUsed->setText(getCpuUsed());
    mMemUsed->setText(getMemUsed());
    getNetUploadDownload();
//    getNetDownload();
    mUptime->setText(getUptime());
}
