#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QSerialPort;

QT_BEGIN_NAMESPACE
namespace Ui
{
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

public slots:
    void onOpen();
    void onReadyRead();

private:
    Ui::MainWindow* ui;
    QSerialPort* _pSerialPort;
    QByteArray _incomingData;
    QVector<double> _valuesY;
    QVector<double> _valuesX;
    quint64 counter;
};
#endif // MAINWINDOW_H
