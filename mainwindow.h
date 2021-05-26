#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QMouseEvent>


class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:

    enum SpeedRateUnit {
        SpeedRateBit,
        SpeedRateByte,
        SpeedRateKb,
        SpeedRateMb,
        SpeedRateGb,
        SpeedRateTb,
        SpeedRateUnknow
    };

    QLabel *mCpuUsed;
    QLabel *mMemUsed;
    QLabel *mNetUpload;
    QLabel *mNetDownload;
    QLabel *mUptime;

    QTimer *timer;

    QString getCpuUsed();
    QString getMemUsed();
    QString getNetUploadDownload();
//    QString getNetDownload();
    QString getUptime();

    QString convertRateUnits(SpeedRateUnit &unit);
    QString autoRateUnits(long speed, SpeedRateUnit &unit, double &sp);
    long old_upload;
    long old_download;

private slots:
    void onTimerout();

protected:
	void mousePressEvent(QMouseEvent *e);
	void mouseMoveEvent(QMouseEvent *e);
	void mouseReleaseEvent(QMouseEvent *e);
};

#endif // MAINWINDOW_H
