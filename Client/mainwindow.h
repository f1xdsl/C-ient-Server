#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>

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
    void on_connectButton_clicked();

    void on_sendButton_clicked();

    void on_lineEdit_returnPressed();

    void on_disconnectButton_clicked();

private:
    Ui::MainWindow *ui;

    QTcpSocket* socket;
    QByteArray Data;
    qint16 nextBlockSize;
    void sendToServer(QString msg);

public slots:
    void slotReadyRead();
    void slotConnected();
    void slotDisconnected();

};
#endif // MAINWINDOW_H
