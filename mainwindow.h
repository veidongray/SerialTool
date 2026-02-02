#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QTimer>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setButtonsEnabled(bool status);

public slots:
    void on_readyReadSerialData();
    void on_timerOut();
private slots:
    void on_pushButtonOpenPort_clicked();

    void on_pushButtonClosePort_clicked();

    void on_pushButtonSendData_clicked();

    void on_pushButtonAbout_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    QTimer *timer;
};
#endif // MAINWINDOW_H
