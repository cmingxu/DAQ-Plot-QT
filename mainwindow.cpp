#include<QDebug>

#include "mainwindow.h"
#include "./ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    deviceState = DeviceState::Disconnected;
    sampleRate = uint32_t(DAQADCSampleRate::SampleRate200K);
    shouldSaveData = false;
    saveDataPathPrefix = "C:\\";

    setupUI();
    updateUI();

    timer_ = new QTimer();
    connect(timer_, &QTimer::timeout, this, &MainWindow::processData);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startBtnClicked(){
    qDebug() << "start" ;
    timer_->setInterval(900);
    timer_->start();

    if(DeviceState::Initialized != deviceState){
        qDebug() << "not initialized";
        return;
    }

    if(!daq122.StartADCCollection() ){
        qDebug() << "start failed";
    }

    deviceState = DeviceState::Connected;
    updateUI();
}

void MainWindow::stopBtnClicked(){
    qDebug() << "stop";

    if(DeviceState::Connected != deviceState) {
        qDebug() << "not in connect state";
        return;
    }

    if(!daq122.StopADCCollection()) {
        qDebug() << "stop failed";
    }


    timer_->stop();
    deviceState = DeviceState::Disconnected;

    updateUI();
}

void MainWindow::sampleRateChanged(){
    qDebug() << "sample rate change";
    QVariant data = ui->sampleRateSampleBox->itemData(ui->sampleRateSampleBox->currentIndex());
    sampleRate = data.value<int>();
    qDebug() << "sample rate change to " << sampleRate;
    ui->rawDataPlot->xAxis->setRange(0, sampleRate);
    ui->rawDataPlot->replot();
    ui->sumDataPlot->xAxis->setRange(0, sampleRate);
    ui->sumDataPlot->replot();
}

void MainWindow::initBtnClicked(){
    qDebug() <<"init";
    if(!daq122.InitializeDevice()) {
        qDebug() <<"initialized failed";
        return;
    }
    if(!daq122.ConnectedDevice()){
        qDebug() <<"connect failed";
        return;
    }
    auto voltage_range = DAQVoltage::Voltage5V;
    if (!daq122.ConfigureADCParameters(DAQADCSampleRate(sampleRate), voltage_range)) {
        qDebug() << "config failed";
    }
    deviceState = DeviceState::Initialized;

    QVector<double> x_data, y_data;
    for(int i = 0; i< 4 ; i ++ ){
        ui->rawDataPlot->graph(i)->setData(x_data, y_data);
    }
    for(int i = 0; i< 3 ; i ++ ){
        ui->sumDataPlot->graph(i)->setData(x_data, y_data);
    }
    ui->fourQuadPlot->graph(0)->setData(x_data, y_data);
    updateUI();
}

void MainWindow::chooseDirectoryBtnClicked(){
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                    saveDataPathPrefix,
                                                    QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks);
    ui->saveDirPathLineEdit->setText(dir);
    saveDataPathPrefix = dir;
}

void MainWindow::saveDataCheckBoxChecked(){
    shouldSaveData = ui->saveCheckbox->isChecked();
    updateSaveWidgets();
}

void MainWindow::processData(){
    qDebug() << "process data";
    auto storage_depth = daq122.GetCurrentSampleRate();
    QVector<QVector<double>> receive_data(8);
    for (int var = 0; var < receive_data.size(); ++var) {
        receive_data[var].resize(storage_depth);
    }

    sums.resize(storage_depth);
    xdiffs.resize(storage_depth);
    ydiffs.resize(storage_depth);
    QVector<double> X, Y;
    X.resize(storage_depth);
    Y.resize(storage_depth);


    if(!daq122.ADCDataIsReady(storage_depth)){
        return;
    }
    auto read_result = true;
    for (int i = 0; i < 8; i++) {
        read_result = daq122.TryReadADCData(i, receive_data[i].data(), receive_data[i].size(), 1000);
        if(!read_result){
            qDebug() << "Error";
        }
    }

    QVector<double> x_data(storage_depth);
    for (int var = 0; var < storage_depth; ++var) {
        x_data[var] = var;
    }

    for (int i = 0; i < storage_depth; ++i) {
        double d1 = qMax(receive_data[0].at(i), 0.0);
        double d2 = qMax(receive_data[1].at(i), 0.0);
        double d3 = qMax(receive_data[2].at(i), 0.0);
        double d4 = qMax(receive_data[3].at(i), 0.0);

        xdiffs[i] = (d2 + d3) - (d1 + d4);
        ydiffs[i] = (d1 + d2) - (d3 + d4);
        sums[i] = (d1 + d2 + d3 + d4);
        X[i] = xdiffs[i] / sums[i];
        Y[i] = ydiffs[i] / sums[i];
    }


    if (read_result) {
        for (int var = 0; var < DEFAULT_CH_CNT; ++var) {
            ui->rawDataPlot->graph(var)->addData(x_data, receive_data[var], true);
        }

        ui->sumDataPlot->graph(0)->addData(x_data, xdiffs, true);
        ui->sumDataPlot->graph(1)->addData(x_data, ydiffs, true);
        ui->sumDataPlot->graph(2)->addData(x_data, sums, true);
        ui->xdiffDisplay->setText(QString("%1").arg(xdiffs.last()));
        ui->ydiffDisplay->setText(QString("%1").arg(ydiffs.last()));
        ui->sumDisplay->setText(QString("%1").arg(sums.last()));

         if(shouldSaveData) {
            for(int var = 0; var < DEFAULT_CH_CNT; ++var) {
                saveData(QString("CH_%1.bin").arg(var + 1), receive_data[var]);
            }
            saveData("XDiffs.bin", xdiffs);
            saveData("YDiffs.bin", ydiffs);
            saveData("Sums.bin", sums);
         }

        QVector<double> lastX, lastY;
        lastX.append(X[storage_depth-1]);
        lastY.append(Y[storage_depth-1]);
        ui->fourQuadPlot->graph(0)->setData(lastX, lastY);

        ui->rawDataPlot->replot(QCustomPlot::rpQueuedReplot);
        ui->sumDataPlot->replot(QCustomPlot::rpQueuedReplot);
        ui->fourQuadPlot->replot();
    }
}

void MainWindow::updateButtonState(){
    switch(deviceState){
    case DeviceState::Disconnected:
        ui->sampleRateSampleBox->setEnabled(true);
        ui->startBtn->setEnabled(false);
        ui->stopBtn->setEnabled(false);
        ui->initBtn->setEnabled(true);
        break;
    case DeviceState::Initialized:
        ui->sampleRateSampleBox->setEnabled(false);
        ui->startBtn->setEnabled(true);
        ui->stopBtn->setEnabled(false);
        ui->initBtn->setEnabled(false);
        break;
    case DeviceState::Connected:
        ui->sampleRateSampleBox->setEnabled(false);
        ui->startBtn->setEnabled(false);
        ui->stopBtn->setEnabled(true);
        ui->initBtn->setEnabled(false);
        break;
    default:
        break;
    }
}

void MainWindow::updateSaveWidgets(){
    if(shouldSaveData) {
        ui->chooseDirPathBtn->setEnabled(true);
    }else{
        ui->chooseDirPathBtn->setEnabled(false);
    }

    ui->saveDirPathLineEdit->setReadOnly(true);
}

void MainWindow::updateUI(){
    updateButtonState();
    updateSaveWidgets();
}

void MainWindow::setupUI() {
    ui->sampleRateSampleBox->addItem("200K", int(DAQADCSampleRate::SampleRate200K));
    ui->sampleRateSampleBox->addItem("100K", int(DAQADCSampleRate::SampleRate100K));
    ui->sampleRateSampleBox->addItem("50K", int(DAQADCSampleRate::SampleRate50K));
    ui->sampleRateSampleBox->addItem("10K", int(DAQADCSampleRate::SampleRate10K));
    ui->sampleRateSampleBox->addItem("5K", int(DAQADCSampleRate::SampleRate5K));
    ui->sampleRateSampleBox->addItem("1K", int(DAQADCSampleRate::SampleRate1K));
    ui->sampleRateSampleBox->addItem("500", int(DAQADCSampleRate::SampleRate500));
    ui->sampleRateSampleBox->addItem("100", int(DAQADCSampleRate::SampleRate100));

    setupRawData();
    setupSumData();
    setupFourQuad();
    connectSlots();
}

void MainWindow::setupRawData(){
    ui->rawDataPlot->xAxis->setRange(0, sampleRate);

    ui->rawDataPlot->yAxis->setLabel(QString("电压 / V"));
    ui->rawDataPlot->yAxis->setVisible(true);
    ui->rawDataPlot->xAxis->setLabel(QString(""));
    ui->rawDataPlot->xAxis->setVisible(true);

    ui->rawDataPlot->yAxis->setRange(-10, 10);
    for(int i = 0; i< DEFAULT_CH_CNT; i ++ ){
        ui->rawDataPlot->addGraph();
    }

    QRandomGenerator generator(QTime::currentTime().msecsSinceStartOfDay());
    for(int i = 0; i< DEFAULT_CH_CNT; i ++ ){
        ui->rawDataPlot->graph(i)->setAdaptiveSampling(true);
        ui->rawDataPlot->graph(i)->setName(QString("CH %1").arg(i + 1));
        ui->rawDataPlot->graph(i)->setPen(QPen(QColor(int(generator.generate() % 255),
                                                      int(generator.generate() % 255),
                                                      int(generator.generate() % 255))));
    }

    ui->rawDataPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->rawDataPlot->yAxis->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->rawDataPlot->yAxis->axisRect()->setRangeZoom(Qt::Horizontal);

    ui->rawDataPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(6); // and make a bit smaller for legend
    ui->rawDataPlot->legend->setFont(legendFont);
    ui->rawDataPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    ui->rawDataPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
}



void MainWindow::setupSumData(){
    // 200k
    ui->sumDataPlot->xAxis->setRange(0, sampleRate);
    ui->sumDataPlot->yAxis->setRange(-20, 20);
    ui->sumDataPlot->yAxis->setLabel(QString("电压 / V"));
    ui->sumDataPlot->yAxis->setVisible(true);
    ui->sumDataPlot->xAxis->setLabel(QString(""));
    ui->sumDataPlot->xAxis->setVisible(true);
    for(int i = 0; i< 3; i ++ ){
        ui->sumDataPlot->addGraph();
    }

    QRandomGenerator generator(QTime::currentTime().msecsSinceStartOfDay() );
     ui->sumDataPlot->graph(0)->setName("XDiffs");
     ui->sumDataPlot->graph(0)->setAdaptiveSampling(true);
     ui->sumDataPlot->graph(0)->setPen(QPen(QColor(int(generator.generate() % 255),
                                                      int(generator.generate() % 255),
                                                      int(generator.generate() % 255))));

     ui->sumDataPlot->graph(1)->setName("YDiffs");
     ui->sumDataPlot->graph(1)->setAdaptiveSampling(true);
     ui->sumDataPlot->graph(1)->setPen(QPen(QColor(int(generator.generate() % 255),
                                                      int(generator.generate() % 255),
                                                      int(generator.generate() % 255))));

     ui->sumDataPlot->graph(2)->setName("Sum");
     ui->sumDataPlot->graph(2)->setAdaptiveSampling(true);
     ui->sumDataPlot->graph(2)->setPen(QPen(QColor(int(generator.generate() % 255),
                                                      int(generator.generate() % 255),
                                                      int(generator.generate() % 255))));

    ui->sumDataPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                QCP::iSelectLegend | QCP::iSelectPlottables);
    ui->sumDataPlot->yAxis->axisRect()->setRangeDrag(Qt::Horizontal);
    ui->sumDataPlot->yAxis->axisRect()->setRangeZoom(Qt::Horizontal);

    ui->sumDataPlot->legend->setVisible(true);
    QFont legendFont = font();  // start out with MainWindow's font..
    legendFont.setPointSize(6); // and make a bit smaller for legend
    ui->sumDataPlot->legend->setFont(legendFont);
    ui->sumDataPlot->legend->setBrush(QBrush(QColor(255,255,255,230)));
    ui->sumDataPlot->axisRect()->insetLayout()->setInsetAlignment(0, Qt::AlignTop|Qt::AlignRight);
    ui->sumDataPlot->rescaleAxes();
}

void MainWindow::setupFourQuad(){
    ui->fourQuadPlot->xAxis->setRange(-1, 1);
    ui->fourQuadPlot->yAxis->setRange(-1, 1);

    ui->fourQuadPlot->addGraph();
    ui->fourQuadPlot->graph(0)->setLineStyle(QCPGraph::lsLine);
    ui->fourQuadPlot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));

    ui->fourQuadPlot->xAxis2->setVisible(true);
    ui->fourQuadPlot->xAxis2->setTickLabels(false);
    ui->fourQuadPlot->yAxis2->setVisible(true);
    ui->fourQuadPlot->yAxis2->setTickLabels(false);

    connect(ui->fourQuadPlot->xAxis, SIGNAL(rangeChanged(QCPRange)),
            ui->fourQuadPlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(ui->fourQuadPlot->yAxis, SIGNAL(rangeChanged(QCPRange)),
            ui->fourQuadPlot->yAxis2, SLOT(setRange(QCPRange)));


}

void MainWindow::connectSlots(){
    connect(ui->rawDataPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
    connect(ui->rawDataPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
    connect(ui->rawDataPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
}


void MainWindow::selectionChanged(){

    qDebug() << "selectionChanged";
}
void MainWindow::mousePress(){

    qDebug() << "mousePress";
}
void MainWindow::mouseWheel(){
    qDebug() << "mouseWheel";
}

void MainWindow::saveData(const QString &filename, const QVector<double> &data)
{
    QDateTime now = QDateTime::currentDateTime();
    QDir dir = QDir(saveDataPathPrefix);
    QFile file = dir.filePath(filename);
    if(file.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QDataStream out(&file);
        for(auto d :data) {
            out << d;
        }
    }
    file.close();
}
