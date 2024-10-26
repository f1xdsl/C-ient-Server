#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>
#include <QTcpSocket>
#include <QVector>
#include <QMap>
#include <QDebug>

class Server: public QTcpServer
{
    Q_OBJECT
public:
    Server();
    QTcpSocket* socket;
private:
    QVector<QTcpSocket*> sockets;
    QByteArray Data;
    quint16 nextBlockSize;
    void sendToClient(QString msg);
public slots:
    void incomingConnection(qintptr socketDescriptor);
    void slotReadyRead();
    void socketDiconnect();
};

#endif // SERVER_H
