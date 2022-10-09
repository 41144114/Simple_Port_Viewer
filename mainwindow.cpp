#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    _pSerialPort = new QSerialPort();
    ui->comboBoxPortName->clear();
    QList<QSerialPortInfo> info = QSerialPortInfo::availablePorts();

    for (int i = 0; i < info.count(); ++i) ui->comboBoxPortName->addItem(info.at(i).portName());
    connect(_pSerialPort, &QSerialPort::readyRead, this, &MainWindow::onReadyRead);
    connect(ui->pushButtonOpen, &QPushButton::clicked, this, &MainWindow::onOpen);
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::onOpen()
{
    if (_pSerialPort->isOpen())
    {
        _pSerialPort->close();
        ui->widgetGraph->clearGraphs();
    }

    _pSerialPort->setPortName(ui->comboBoxPortName->currentText());
    _pSerialPort->open(QIODevice::ReadOnly);
    _pSerialPort->setBaudRate(QSerialPort::Baud115200);
    _pSerialPort->setDataBits(QSerialPort::Data8);
    _pSerialPort->setStopBits(QSerialPort::OneStop);
    _pSerialPort->setFlowControl(QSerialPort::NoFlowControl);
    counter = 0;
    ui->widgetGraph->addGraph(ui->widgetGraph->xAxis, ui->widgetGraph->yAxis);
}

void MainWindow::onReadyRead()
{
    _incomingData.append(_pSerialPort->readAll());
    while (_incomingData.contains("\r\n"))
    {
        int index = _incomingData.indexOf("\r\n");
        int value = _incomingData.mid(0, index).toInt();
        counter++;
        _incomingData.remove(0, index + 2);

        ui->widgetGraph->graph(0)->addData(counter, value);
        ui->widgetGraph->xAxis->rescale();
        ui->widgetGraph->graph(0)->rescaleValueAxis();
        ui->widgetGraph->replot();
        ui->lineEditValue->setText(QString::number(value));
    }
}
