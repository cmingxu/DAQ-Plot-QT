#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <libdaq/device/DAQ122/daq122.h>
using namespace libdaq::device;


enum DeviceState {
    Disconnected = 0,
    Initialized = 1,
    Connected = 2,
};

static const quint8 DEFAULT_CH_CNT = 4;
static const bool DEBUG = true;

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void initBtnClicked();
    void startBtnClicked();
    void stopBtnClicked();
    void sampleRateChanged();

    void saveDataCheckBoxChecked();
    void chooseDirectoryBtnClicked();


    void processData();
    void mousePress();
    void mouseWheel();
    void selectionChanged();

private:
    void updateUI();
    void updateButtonState();
    void updateSaveWidgets();

    void setupUI();
    void setupRawData();
    void setupSumData();
    void setupFourQuad();
    void connectSlots();

    void saveData(const QString& filename, const QVector<double> &data);

private:
    Ui::MainWindow *ui;
    DAQ122 daq122;
    QTimer *timer_;


    DeviceState deviceState;
    uint32_t sampleRate;
    QVector<double> sums;
    QVector<double> xdiffs;
    QVector<double> ydiffs;

    bool shouldSaveData;
    QString saveDataPathPrefix;

};
#endif // MAINWINDOW_H
