#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QtDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include <QStack>
#include <QDateTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBoxBaudRate->setCurrentText(tr("115200"));
    ui->comboBoxDataBits->setCurrentText(tr("8"));

    /* Add serial ports */
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo port : ports) {
        ui->comboBoxPort->addItem(port.portName());
        qDebug() << port.portName();
    }

    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::on_timerOut);
    timer->start(1000);

    timerShow = new QTimer(this);
    connect(timerShow, &QTimer::timeout, this, &MainWindow::on_timerShowOut);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_timerShowOut
 * 每100ms输出一次rxBuffer的内容
 */
void MainWindow::on_timerShowOut()
{
    if (!rxBuffer->isEmpty()) {
        QString strRead;
        if (ui->checkBoxTimeView->isChecked()) {
            QDateTime current = QDateTime::currentDateTime();
            ui->textEditRead->moveCursor(QTextCursor::End);
            strRead = "Recv[" + current.toString("HH:mm:ss") + "]:\r\n";
            ui->textEditRead->append(tr("<font color='blue'>%1</font>").arg(strRead));
        } else {
            strRead = "Recv:\r\n";
            ui->textEditRead->append(tr("<font color='blue'>%1</font>").arg(strRead));
        }
        ui->textEditRead->append(*rxBuffer);
        rxBuffer->clear();
        ui->textEditRead->moveCursor(QTextCursor::End);
    }
}

/**
 * @brief MainWindow::on_timerOut
 * 每1s检查并更新一次串口列表
 * 如果当前串口被打开则不会更新
 */
void MainWindow::on_timerOut()
{
    QStack<QString> stack;

    stack.push(ui->comboBoxPort->currentText());
    ui->comboBoxPort->clear();
    QList<QSerialPortInfo> ports = QSerialPortInfo::availablePorts();
    for (QSerialPortInfo port : ports) {
        ui->comboBoxPort->addItem(port.portName());
        qDebug() << port.portName();
    }
    /* Prefer the previous port if it exists in the current port list */
    for (int i = 0; i < ports.length(); i++) {
        if (ports[i].portName() == stack.top()) {
            ui->comboBoxPort->setCurrentText(stack.top());
            break;
        }
    }
}

/**
 * @brief MainWindow::setButtonsEnabled
 * @param status
 * 配置各个按钮以及下拉框的可选状态
 * 分为串口被打开时和串口关闭时的不同状态
 */
void MainWindow::setButtonsEnabled(bool status)
{
    if (status == true) {
        /* status == true means opened serial port */
        ui->comboBoxPort->setEnabled(false);
        ui->comboBoxDataBits->setEnabled(false);
        ui->comboBoxBaudRate->setEnabled(false);
        ui->comboBoxStopBits->setEnabled(false);
        ui->comboBoxParity->setEnabled(false);

        ui->pushButtonOpenPort->setEnabled(false);
        ui->pushButtonSendData->setEnabled(true);
        ui->pushButtonClosePort->setEnabled(true);
    } else {
        /* status == false means closed serial port */
        ui->comboBoxPort->setEnabled(true);
        ui->comboBoxDataBits->setEnabled(true);
        ui->comboBoxBaudRate->setEnabled(true);
        ui->comboBoxStopBits->setEnabled(true);
        ui->comboBoxParity->setEnabled(true);

        ui->pushButtonOpenPort->setEnabled(true);
        ui->pushButtonSendData->setEnabled(false);
        ui->pushButtonClosePort->setEnabled(false);
    }
}

/**
 * @brief MainWindow::on_pushButtonOpenPort_clicked
 * 打开串口，打开之前会对串口进行配置
 */
void MainWindow::on_pushButtonOpenPort_clicked()
{
    serial = new QSerialPort(this);

    serial->setPortName(ui->comboBoxPort->currentText());
    switch (ui->comboBoxBaudRate->currentText().toInt()) {
    case 1200:
        serial->setBaudRate(QSerialPort::Baud1200);
        break;
    case 2400:
        serial->setBaudRate(QSerialPort::Baud2400);
        break;
    case 4800:
        serial->setBaudRate(QSerialPort::Baud4800);
        break;
    case 9600:
        serial->setBaudRate(QSerialPort::Baud9600);
        break;
    case 19200:
        serial->setBaudRate(QSerialPort::Baud19200);
        break;
    case 38400:
        serial->setBaudRate(QSerialPort::Baud38400);
        break;
    case 57600:
        serial->setBaudRate(QSerialPort::Baud57600);
        break;
    case 115200:
        serial->setBaudRate(QSerialPort::Baud115200);
        break;
    default:
        serial->setBaudRate(QSerialPort::UnknownBaud);
    }

    switch (ui->comboBoxDataBits->currentText().toInt()) {
    case 5:
        serial->setDataBits(QSerialPort::Data5);
        break;
    case 6:
        serial->setDataBits(QSerialPort::Data6);
        break;
    case 7:
        serial->setDataBits(QSerialPort::Data7);
        break;
    case 8:
        serial->setDataBits(QSerialPort::Data8);
        break;
    default:
        serial->setDataBits(QSerialPort::UnknownDataBits);
    }

    switch (ui->comboBoxParity->currentText().toUtf8()[0]) {
    case 'N':
        serial->setParity(QSerialPort::NoParity);
        break;
    case 'E':
        serial->setParity(QSerialPort::EvenParity);
        break;
    case 'O':
        serial->setParity(QSerialPort::OddParity);
        break;
    case 'M':
        serial->setParity(QSerialPort::MarkParity);
        break;
    case 'S':
        serial->setParity(QSerialPort::SpaceParity);
        break;
    default:
        serial->setParity(QSerialPort::UnknownParity);
    }

    if (ui->comboBoxStopBits->currentText().toDouble() == 1) {
        serial->setStopBits(QSerialPort::OneStop);
    } else if (ui->comboBoxStopBits->currentText().toDouble() == 1.5) {
        serial->setStopBits(QSerialPort::OneAndHalfStop);
    } else if (ui->comboBoxStopBits->currentText().toDouble() == 2) {
        serial->setStopBits(QSerialPort::TwoStop);
    } else {
        serial->setStopBits(QSerialPort::UnknownStopBits);
    }

    qDebug() << serial->portName();
    qDebug() << serial->baudRate();
    qDebug() << serial->dataBits();
    qDebug() << serial->parity();
    qDebug() << serial->stopBits();

    if (serial->open(QIODevice::ReadWrite)) {
        timer->stop();
        this->setButtonsEnabled(true);
        rxBuffer = new QByteArray(1024 * 1024, 0x00);   // 1MB
        rxBuffer->clear();
        serial->setReadBufferSize(1024 * 1024); // 1MB
        timerShow->start(100);
        connect(serial, &QSerialPort::readyRead, this, &MainWindow::on_readyReadSerialData);
        qDebug() << tr("Open port successfully!");
    } else {
        timer->start(1000);
        QMessageBox::warning(this, tr("Warning"), serial->errorString());
        qDebug() << serial->errorString();
    }
}

/**
 * @brief MainWindow::on_pushButtonClosePort_clicked
 * 关闭串口，重新打开更新串口列表的时钟，清空rxBuffer的内容
 */
void MainWindow::on_pushButtonClosePort_clicked()
{
    timer->start(1000);
    timerShow->stop();
    serial->close();
    rxBuffer->clear();
    delete rxBuffer;
    disconnect(serial, &QSerialPort::readyRead, this, &MainWindow::on_readyReadSerialData);
    delete serial;
    serial = nullptr;
    this->setButtonsEnabled(false);
    qDebug() << tr("Close port successfully!");
}

void MainWindow::on_readyReadSerialData()
{
    rxBuffer->append(serial->readAll());
}

/**
 * @brief MainWindow::on_pushButtonSendData_clicked
 * 发送数据
 */
void MainWindow::on_pushButtonSendData_clicked()
{
    QString strRead;
    QString strData = ui->plainTextEditWrite->toPlainText();
    if (strData.isEmpty())
        return;
    if (serial->write(strData.toUtf8()) < 0)
        qDebug() << serial->errorString();
    else {
        if (ui->checkBoxTimeView->isChecked()) {
            QDateTime current = QDateTime::currentDateTime();
            ui->textEditRead->moveCursor(QTextCursor::End);
            strRead = "Send[" + current.toString("HH:mm:ss") + "]:\r\n";
            ui->textEditRead->append(tr("<font color='blue'>%1</font>").arg(strRead));
        } else {
            strRead = "Send:\r\n";
            ui->textEditRead->append(tr("<font color='blue'>%1</font>").arg(strRead));
        }
        ui->textEditRead->append(tr("<font color='black'>%1</font>").arg(strData));
    }
    ui->textEditRead->moveCursor(QTextCursor::End);
}

void MainWindow::on_pushButtonAbout_clicked()
{
    QMessageBox::about(this, tr("About Serial Tool"),
            tr("<h3>Serial Tool</h3>"
               "<p>Version: 1.1.2</p>"
               "<p>Copyright © 2026 Veidongray@qq.com</p>"
               "<p>Built with Qt %1</p>")
                       .arg(QT_VERSION_STR));
}

void MainWindow::on_pushButtonClear_clicked()
{
    ui->textEditRead->clear();
}
