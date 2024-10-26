#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    nextBlockSize = 0;
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::slotDisconnected);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::slotConnected);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotConnected(){
    ui->connectButton->setEnabled(false);
    ui->disconnectButton->setEnabled(true);
    ui->sendButton->setEnabled(true);
    ui->lineEdit->setEnabled(true);
}
void MainWindow::slotDisconnected(){
    socket->deleteLater();
    ui->connectButton->setEnabled(true);
    ui->disconnectButton->setEnabled(false);
    ui->sendButton->setEnabled(false);
    ui->lineEdit->setEnabled(false);

}

int cnt = 0;
void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_6_2);
    if(in.status() == QDataStream::Ok)
    {
        for(;;)
        {
            if(nextBlockSize == 0)
            {
                if(socket->bytesAvailable() < 2){
                    break;
                }
                in>>nextBlockSize;
            }
            if(socket->bytesAvailable() < nextBlockSize)
            {
                break;
            }

            QString str;
            in>>str;
            nextBlockSize = 0;
            ui->textBrowser->append(str);
            ui->textBrowser->setAlignment(Qt::AlignLeft);
        }
    }
    else
    {
        ui->textBrowser->append("read error");
        ui->textBrowser->setAlignment(Qt::AlignCenter);
    }
}

void MainWindow::sendToServer(QString msg){
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);

    out.setVersion(QDataStream::Qt_6_2);
    out<<qint16(0)<<msg;
    out.device()->seek(0);
    out<<qint16(Data.size() - sizeof(qint16));
    socket->write(Data);
    ui->lineEdit->clear();
    ui->textBrowser->append(msg);
    ui->textBrowser->setAlignment(Qt::AlignRight);
}


void MainWindow::on_sendButton_clicked()
{
    sendToServer(ui->lineEdit->text());
}


void MainWindow::on_lineEdit_returnPressed()
{
    sendToServer(ui->lineEdit->text());
}

void MainWindow::on_connectButton_clicked()
{
    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &MainWindow::slotReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &MainWindow::slotDisconnected);
    connect(socket, &QTcpSocket::connected, this, &MainWindow::slotConnected);
    socket->connectToHost("127.0.0.1", 1919);
 }

void MainWindow::on_disconnectButton_clicked()
{
    if(socket->state()==QAbstractSocket::ConnectedState)
    {
        socket->disconnectFromHost();
    }
}


